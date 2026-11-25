// db.cpp
#include "db.h"

std::string Db::escape(const std::string& s) {
    std::string out; out.reserve(s.size() + 8);
    for (char c : s) {
        switch (c) {
            case '\0': out += "\\0";  break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\\': out += "\\\\"; break;
            case '\'': out += "\\'";  break;
            case '"':  out += "\\\""; break;
            case '\x1a': out += "\\Z"; break;
            default:   out.push_back(c);
        }
    }
    return out;
}
