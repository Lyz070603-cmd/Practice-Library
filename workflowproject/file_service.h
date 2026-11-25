// file_service.h
#pragma once
#include <string>

class FileService {
public:
    static std::string sha1Hex(const std::string& data);
    static std::string category(const std::string& filename);
    static std::string storagePathFromSha1(const std::string& sha1);   // "aa/bbbb..."
    static std::string absolutePath(const std::string& sha1);          // "storage/aa/bbbb..."
    static std::string uuid4();
};
