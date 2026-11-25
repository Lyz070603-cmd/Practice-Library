#include <iostream>
#include <string>
#include <alibabacloud/oss/OssClient.h>
#include <fcntl.h>
#include <fstream>
using namespace std;
using namespace AlibabaCloud::OSS;

struct OSSInfo{
    string OSSbucket;
    string OSSEndPoint;
    string OSSAcessKeyID;
    string OSSAcessKeySercet;
};
bool loadOSSInfo(const std::string &path, OSSInfo &info) {
    std::ifstream ifs(path, std::ios::binary); // 用二进制模式，避免换行转换干扰
    if (!ifs.is_open()) { /* ... */ return false; }

    if (!std::getline(ifs, info.OSSbucket)      ||
        !std::getline(ifs, info.OSSEndPoint)    ||
        !std::getline(ifs, info.OSSAcessKeyID)  ||
        !std::getline(ifs, info.OSSAcessKeySercet)) {
        /* ... */ return false;
    }

    auto clean = [](std::string &s) {
        // 去掉行尾的 \r (Windows 换行)
        while (!s.empty() && s.back() == '\r') s.pop_back();
        // 去掉首尾空格
        size_t start = s.find_first_not_of(" \t");
        size_t end   = s.find_last_not_of(" \t");
        s = (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
        // 去掉 UTF-8 BOM
        if (s.size() >= 3 && (unsigned char)s[0] == 0xEF
                         && (unsigned char)s[1] == 0xBB
                         && (unsigned char)s[2] == 0xBF) {
            s = s.substr(3);
        }
    };

    clean(info.OSSbucket);
    clean(info.OSSEndPoint);
    clean(info.OSSAcessKeyID);
    clean(info.OSSAcessKeySercet);
    return true;
}

int main()
{
    InitializeSdk();
    OSSInfo ossInfo;
    loadOSSInfo("./OSS_info.txt",ossInfo);

    ClientConfiguration conf;
    conf.signatureVersion = SignatureVersionType::V4;

    OssClient client(ossInfo.OSSEndPoint,
                     ossInfo.OSSAcessKeyID,
                     ossInfo.OSSAcessKeySercet,conf);
    client.SetRegion("cn-beijing");
    string path = "./text.txt";

    /* 上传文件。*/
    /* 填写本地文件完整路径，例如D:\\localpath\\examplefile.txt，其中localpath为本地文件examplefile.txt所在本地路径。*/
    auto outcome = client.PutObject(ossInfo.OSSbucket,"dir/text.txt",path);

    if (!outcome.isSuccess()) {
        /* 异常处理。*/
        std::cout << "PutObject fail" <<
        ",code:" << outcome.error().Code() <<
        ",message:" << outcome.error().Message() <<
        ",requestId:" << outcome.error().RequestId() << std::endl;
        return -1;
    }

    /* 释放网络等资源。*/
    ShutdownSdk();

    return 0;
}

