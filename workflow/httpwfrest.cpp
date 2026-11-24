#include <wfrest/HttpServer.h>
#include <wfrest/HttpContent.h>
#include <wfrest/Json.h>
#include <wfrest/HttpServerTask.h>
#include <workflow/WFFacilities.h>
#include <iostream>
using namespace wfrest;
using std::string;
using std::cout;
using std::endl;
static WFFacilities::WaitGroup wait_group(1);
void sighandler(int signal){
    wait_group.done();
    cout << "server stop" << endl;
    exit(0);
}

int main (){
    HttpServer server;
    server.GET("/login",[](const HttpReq *req, HttpResp *resp){
        resp->File("text.html");                
    });
    server.POST("/login",[](const HttpReq * req,HttpResp * resp){

        resp->File("hello.html");
    });

    if (server.start(1234) == 0){
        wait_group.wait();
        server.stop();
        return 0;
    }
    else{
        cout << "server start error" << endl;
        return 0;

    }

    return 0;
}
