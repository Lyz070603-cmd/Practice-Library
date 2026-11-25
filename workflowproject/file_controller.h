// file_controller.h
#pragma once
#include <wfrest/HttpServer.h>

class FileController {
public:
    void upload   (const wfrest::HttpReq* req, wfrest::HttpResp* resp);
    void download (const wfrest::HttpReq* req, wfrest::HttpResp* resp);
    void list     (const wfrest::HttpReq* req, wfrest::HttpResp* resp);
    void remove   (const wfrest::HttpReq* req, wfrest::HttpResp* resp);
    void checkSha1(const wfrest::HttpReq* req, wfrest::HttpResp* resp);
};
