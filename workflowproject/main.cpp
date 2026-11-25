#include <wfrest/HttpServer.h>
#include <workflow/WFFacilities.h>
#include <csignal>
#include <iostream>

#include "config.h"
#include "user_controller.h"
#include "file_controller.h"

using namespace wfrest;
using std::cout;
using std::endl;

static WFFacilities::WaitGroup g_wait(1);

static void handleSignal(int) {
    g_wait.done();
    cout << "server stopping..." << endl;
    exit(0);
}

int main() {
    signal(SIGINT,  handleSignal);
    signal(SIGTERM, handleSignal);

    HttpServer server;
    UserController userCtrl;
    FileController fileCtrl;

    // 静态页面
    server.GET("/",      [](const HttpReq*, HttpResp* resp){ resp->File(Config::wwwRoot() + "/login.html"); });
    server.GET("/login", [](const HttpReq*, HttpResp* resp){ resp->File(Config::wwwRoot() + "/login.html"); });
    server.GET("/home",  [](const HttpReq*, HttpResp* resp){ resp->File(Config::wwwRoot() + "/index.html"); });
    server.GET("/error", [](const HttpReq*, HttpResp* resp){ resp->File(Config::wwwRoot() + "/error.html"); });

    // 认证 API —— 用 lambda 代替 std::bind
    server.POST("/api/register", [&userCtrl](const HttpReq* req, HttpResp* resp){
        userCtrl.registerUser(req, resp);
    });
    server.POST("/api/login", [&userCtrl](const HttpReq* req, HttpResp* resp){
        userCtrl.login(req, resp);
    });

    // 文件 API
    server.POST("/api/upload", [&fileCtrl](const HttpReq* req, HttpResp* resp){
        fileCtrl.upload(req, resp);
    });
    server.GET("/api/download/:id", [&fileCtrl](const HttpReq* req, HttpResp* resp){
        fileCtrl.download(req, resp);
    });
    server.GET("/api/list", [&fileCtrl](const HttpReq* req, HttpResp* resp){
        fileCtrl.list(req, resp);
    });
    server.DELETE("/api/file/:id", [&fileCtrl](const HttpReq* req, HttpResp* resp){
        fileCtrl.remove(req, resp);
    });
    server.GET("/api/check_sha1", [&fileCtrl](const HttpReq* req, HttpResp* resp){
        fileCtrl.checkSha1(req, resp);
    });

    if (server.start(1234) == 0) {
        server.list_routes();
        g_wait.wait();
        server.stop();
    } else {
        cout << "server start error" << endl;
        return 1;
    }
    return 0;
}
