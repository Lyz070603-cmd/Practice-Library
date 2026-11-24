#include <wfrest/HttpServer.h>
#include <wfrest/HttpContent.h>
#include <wfrest/Json.h>
#include <wfrest/HttpServerTask.h>
#include <workflow/WFFacilities.h>
#include <functional>
#include <iostream>
#include <map>
using namespace wfrest;
using std::string;
using std::cout;
using std::endl;
using std::map;
static WFFacilities::WaitGroup wait_group(1);
void sighandler(int signal){
    wait_group.done();
    cout << "server stop" << endl;
    exit(0);
}

void login_handler(const HttpReq *req, HttpResp *resp) {
    resp->File("text.html");
}
void login_in_handler(const HttpReq *req, HttpResp *resp) {
     if (req->content_type() != APPLICATION_URLENCODED) {
        resp->String("<h2>请用表单提交</h2>");
        return;
    }

    // 2. 拿到客户端发来的用户名和密码
    auto body = req->form_kv();
    std::string username = body["username"];
    std::string password = body["password"];

    std::cout << "登录尝试: " << username << " / " << password << std::endl;

    // 3. 简单校验：写死一个账号密码
    if (username == "admin" && password == "123456") {
        // 正确 → 返回登录后页面
        resp->File("hello.html");
    } else {
        // 错误 → 返回提示页
        resp->add_header_pair("Content-Type", "text/html; charset=utf-8");
        resp->String("<!DOCTYPE html><h2>用户名或密码错误</h2><a href='/login'>返回重新登录</a>");
    }
}
int main (){
    HttpServer server;
    Handler login = std::bind(login_handler,
                    std::placeholders::_1,std::placeholders::_2);
    Handler login_in = std::bind(login_in_handler,
                    std::placeholders::_1,std::placeholders::_2);


    server.GET("/login",login);
    server.POST("/login",login_in);
    if (server.start(1234) == 0){
        server.list_routes();
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
