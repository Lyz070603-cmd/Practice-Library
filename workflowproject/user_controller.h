// user_controller.h
#pragma once
#include <wfrest/HttpServer.h>

class UserController {
public:
    void login(const wfrest::HttpReq* req, wfrest::HttpResp* resp);
    void registerUser(const wfrest::HttpReq* req, wfrest::HttpResp* resp);
};
