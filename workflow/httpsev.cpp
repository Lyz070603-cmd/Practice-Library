#include <workflow/WFHttpServer.h>
#include <workflow/WFFacilities.h>
#include <workflow/HttpUtil.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <signal.h>
#include <string>

using Json = nlohmann::json;

using std::cout;
using std::endl;
using std::string;

// http://192.168.118.128:1234/file/mupload/init
// http://192.168.118.128:1234/file/mupload/uppart?uploadID=liao1122&chkidx=2
// http://192.168.118.128:1234/file/mupload/complete?uploadID=liao1122

static WFFacilities::WaitGroup wait_group(1);
void sighandler(int signo){
    wait_group.done();
    cout << "sever stop" << endl;
    return ;
}

void process(WFHttpTask *task){
    auto resp = task->get_resp();
    auto req  = task->get_req();
    
    string uri = req->get_request_uri();
    string method = req->get_method();
    string path= uri.substr(0,uri.find("?"));
    string query= uri.substr(uri.find("?")+1);
    
    if (path == "/file/mupload/init" && method == "POST"){
        const void * body;
        size_t size;
        req->get_parsed_body(&body,&size);
        cout << " succes" << "\n";

        Json fileInfo = Json::parse(static_cast<const char *>(body));
         std::string filename = fileInfo["filename"];
        int filesize = fileInfo["filesize"];
        std::string filehash = fileInfo["filehash"];
        //fprintf(stderr,"%s %s %d\n", filename.c_str(), filehash.c_str(), filesize);
        // 3 初始化分块操作
        // 生成uploadID
        std::string uploadID = "liao";
        time_t now = time(NULL);
        struct tm *ptm = localtime(&now);
        char timeStamp[30] = {0};
        sprintf(timeStamp,"%02d%02d%02d", ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
        uploadID = uploadID + timeStamp;
        //fprintf(stderr,"%s\n", uploadID.c_str());
        // 生成分块信息
        int chunkcount;
        int chunksize = 1024*1024;
        if(filesize % chunksize != 0){
            chunkcount = filesize/chunksize + 1;
        }
        else{
            chunkcount = filesize/chunksize;
        }
        //fprintf(stderr,"chunksize = %d, chunkcount = %d\n", chunksize, chunkcount);
        // 初始化redis
        // HSET uploadID chunkcount 6
        // HSET uploadID filehash xxx
        // HSET uploadID filesize xxx
        std::vector<std::vector<std::string>> argsVec = {
            {uploadID, "chunkcount", std::to_string(chunkcount)},
            {uploadID, "filehash", filehash},
            {uploadID, "filesize", std::to_string(filesize)}
        };
        for(int i = 0; i < 3; ++i){
            auto redisTask = WFTaskFactory::create_redis_task("redis://localhost",0,nullptr);
            redisTask->get_req()->set_request("HSET", argsVec[i]);
            redisTask->start();
        }
        // 生成响应给客户端
        Json uppartInfo;
        uppartInfo["status"] = "OK";
        uppartInfo["uploadID"] = uploadID;
        uppartInfo["chunkcount"] = chunkcount;
        uppartInfo["filesize"] = filesize;
        uppartInfo["chunksize"] = chunksize;
        uppartInfo["filehash"] = filehash;
        resp->append_output_body(uppartInfo.dump());


    }
}



int main (){
    signal(SIGINT,sighandler);
    WFHttpServer sever(process);
    if (sever.start(1234) == 0){
        wait_group.wait();
        sever.stop();
    }
    else {
        perror("sever start error!\n");
        exit(-1);
    }

    return 0;
}
