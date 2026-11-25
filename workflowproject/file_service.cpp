// file_service.cpp
#include "file_service.h"
#include "config.h"
#include <openssl/evp.h>
#include <algorithm>
#include <random>
#include <sstream>

std::string FileService::sha1Hex(const std::string& data) {
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int dlen = 0;
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha1(), nullptr);
    EVP_DigestUpdate(ctx, data.data(), data.size());
    EVP_DigestFinal_ex(ctx, digest, &dlen);
    EVP_MD_CTX_free(ctx);

    static const char* hex = "0123456789abcdef";
    std::string out(dlen * 2, '\0');
    for (unsigned int i = 0; i < dlen; ++i) {
        out[i*2]   = hex[digest[i] >> 4];
        out[i*2+1] = hex[digest[i] & 0x0F];
    }
    return out;
}

std::string FileService::category(const std::string& name) {
    auto dot = name.rfind('.');
    if (dot == std::string::npos) return "other";
    std::string ext = name.substr(dot + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    if (ext=="jpg"||ext=="jpeg"||ext=="png"||ext=="gif"||ext=="webp"||ext=="bmp") return "image";
    if (ext=="pdf"||ext=="doc"||ext=="docx"||ext=="txt"||ext=="md"||
        ext=="ppt"||ext=="pptx"||ext=="xls"||ext=="xlsx") return "doc";
    if (ext=="mp4"||ext=="mkv"||ext=="mov"||ext=="avi"||ext=="webm") return "video";
    if (ext=="mp3"||ext=="wav"||ext=="flac"||ext=="aac"||ext=="ogg") return "audio";
    return "other";
}

std::string FileService::storagePathFromSha1(const std::string& sha1) {
    return sha1.substr(0, 2) + "/" + sha1.substr(2);
}

std::string FileService::absolutePath(const std::string& sha1) {
    return Config::storageRoot() + "/" + storagePathFromSha1(sha1);
}

std::string FileService::uuid4() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<int> dis(0, 15);
    std::ostringstream oss; oss << std::hex;
    for (int i = 0; i < 32; ++i) oss << dis(gen);
    return oss.str();
}
