#pragma once
#include <string>

struct Config {
    static std::string mysqlUrl()    { return "mysql://root:111111@127.0.0.1:3306/cloudbox"; }
    static std::string redisUrl()    { return "redis://127.0.0.1:6379/0"; }
    static std::string storageRoot() { return "storage"; }
    static std::string wwwRoot()     { return "www"; }

    // ★ token 签名盐，生产环境务必修改
    static std::string tokenSalt()   { return "cloudbox-token-salt-2025-change-me"; }
};
