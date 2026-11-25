// db.h
#pragma once
#include <string>
#include "config.h"

class Db {
public:
    static const std::string& mysqlUrl() { static const std::string u = Config::mysqlUrl(); return u; }
    static const std::string& redisUrl() { static const std::string u = Config::redisUrl(); return u; }
    static std::string escape(const std::string& s);
};
