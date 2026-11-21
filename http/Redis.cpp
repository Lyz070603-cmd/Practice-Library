#include <iostream>
#include <string>
#include <hiredis/hiredis.h>

using namespace std;

string findfinalvalue(redisContext *c,const string &key){
    string result;

    redisReply *reply = (redisReply *)redisCommand(c,"GET %s",key.c_str());
    if (reply && reply->type == REDIS_REPLY_STRING){
        result = reply->str;
        freeReplyObject(reply);
    }
    else {
        freeReplyObject(reply);
        return 0;
    }

    while(result != "100"){
        reply = (redisReply*)redisCommand(c,"Get %s",result.c_str());
        if (reply && reply->type == REDIS_REPLY_STRING){
            result = reply->str;
            freeReplyObject(reply);
        }
        else {
            freeReplyObject(reply);
            return "";
        }
    }

    return result;
}

int main (){
    redisContext * c = redisConnect("127.0.0.1",6379);
    if (c == nullptr || c->err){
        if (c){
            cerr << "error" << c->errstr << endl;
            redisFree(c);
        }
        else {
            cerr << "error" << endl;
        }
        return 1;
    }

    string key = "x1";
    string finalvalue = findfinalvalue(c,key);
    
    if (!finalvalue.empty()) {
        std::cout << "Final value found: " << finalvalue << std::endl;
    } else {
        std::cerr << "Error: Final value not found!" << std::endl;
    }

    redisFree(c);

}
