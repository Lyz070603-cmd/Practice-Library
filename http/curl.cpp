#include <iostream>
#include <curl/curl.h>
#include <hiredis/hiredis.h>

size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

int main (){
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Error: Failed to initialize curl" << std::endl;
        return 1;
    }
    std::string url = "http://www.taobao.com";
    std::string htmlContent;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlContent);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Error: Failed to get website content: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 1;
    }
    redisContext *redisContext = redisConnect("127.0.0.1", 6379);
    if (redisContext == NULL || redisContext->err) {
        if (redisContext) {
            std::cerr << "Error: " << redisContext->errstr << std::endl;
            redisFree(redisContext);
        } else {
            std::cerr << "Error: Can't allocate redis context" << std::endl;
        }
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 1;
    }
    redisReply *reply = (redisReply*)redisCommand(redisContext, "SET %s %b",
                url.c_str(), htmlContent.data(), htmlContent.size());
    if (!reply || redisContext->err) {
        std::cerr << "Error: Failed to set website content to Redis" << std::endl;
        freeReplyObject(reply);
        redisFree(redisContext);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 1;
    }
    freeReplyObject(reply);

    std::cout << "Website content stored in Redis successfully" << std::endl;

    // 断开连接并释放资源
    redisFree(redisContext);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}
