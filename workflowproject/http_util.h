#pragma once
#include <wfrest/HttpMsg.h>

inline void sendJson(wfrest::HttpResp* resp, const std::string& body) {
    resp->add_header_pair("Content-Type", "application/json; charset=utf-8");
    resp->append_output_body(body.c_str(), body.size());
}
