// auth.h
#pragma once
#include <string>
#include <wfrest/HttpMsg.h>

class Auth {
public:
    // token = <uid>.<ts>.<sha256(uid.ts.salt)>
    static std::string generate(long userId, const std::string& username);
    // 成功返回 uid，失败返回 -1
    static long verify(const wfrest::HttpReq* req);
    static std::string sha256(const std::string& data);
};
