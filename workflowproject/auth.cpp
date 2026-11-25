// auth.cpp
#include "auth.h"
#include "config.h"
#include <openssl/sha.h>
#include <ctime>

std::string Auth::sha256(const std::string& data) {
    unsigned char h[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(data.data()), data.size(), h);
    static const char* hex = "0123456789abcdef";
    std::string out(SHA256_DIGEST_LENGTH * 2, '\0');
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        out[i*2]   = hex[h[i] >> 4];
        out[i*2+1] = hex[h[i] & 0x0f];
    }
    return out;
}

std::string Auth::generate(long userId, const std::string& /*username*/) {
    long ts = static_cast<long>(std::time(nullptr));
    std::string payload = std::to_string(userId) + "." + std::to_string(ts);
    std::string sig     = sha256(payload + "." + Config::tokenSalt());
    return payload + "." + sig;
}

long Auth::verify(const wfrest::HttpReq* req) {
    if (!req->has_header("Authorization")) return -1;
    std::string auth = req->header("Authorization");
    const std::string prefix = "Bearer ";
    if (auth.rfind(prefix, 0) != 0) return -1;
    std::string token = auth.substr(prefix.size());

    auto p1 = token.find('.');
    if (p1 == std::string::npos) return -1;
    auto p2 = token.find('.', p1 + 1);
    if (p2 == std::string::npos) return -1;

    std::string uidStr = token.substr(0, p1);
    std::string tsStr  = token.substr(p1 + 1, p2 - p1 - 1);
    std::string sig    = token.substr(p2 + 1);

    long uid, ts;
    try {
        uid = std::stol(uidStr);
        ts  = std::stol(tsStr);
    } catch (...) { return -1; }

    if (std::time(nullptr) - ts > 7L * 24 * 3600) return -1;

    std::string expect = sha256(uidStr + "." + tsStr + "." + Config::tokenSalt());
    if (expect != sig) return -1;
    return uid;
}
