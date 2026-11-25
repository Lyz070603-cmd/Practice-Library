// file_controller.cpp
#include "file_controller.h"
#include "auth.h"
#include "db.h"
#include "config.h"
#include "file_service.h"
#include "http_util.h"
#include <wfrest/HttpContent.h>
#include <wfrest/PathUtil.h>
#include <fstream>
#include <filesystem>
#include <memory>

using namespace wfrest;
namespace fs = std::filesystem;

// ---------------- 上传 ----------------
void FileController::upload(const HttpReq* req, HttpResp* resp) {
    long uid = Auth::verify(req);
    if (uid < 0) { resp->set_status(HttpStatusUnauthorized); resp->String("unauthorized"); return; }

    const auto& form = req->form();
    if (form.empty()) { resp->String("no file"); return; }

    // 一次请求只处理第一个文件，前端也限制单文件
    auto it = form.begin();
    std::string origName = it->second.first;
    auto content = std::make_shared<std::string>(it->second.second);

    std::string safeName = PathUtil::base(origName);
    if (safeName.empty()) safeName = "unnamed";

    std::string category = FileService::category(safeName);
    std::string sha1     = FileService::sha1Hex(*content);
    std::string relPath  = FileService::storagePathFromSha1(sha1);
    std::string absPath  = FileService::absolutePath(sha1);
    long size = static_cast<long>(content->size());

    // 秒传判定
    std::string checkSql =
        "SELECT id FROM file_meta WHERE user_id=" + std::to_string(uid) +
        " AND sha1='" + sha1 + "' LIMIT 1";

    resp->MySQL(Db::mysqlUrl(), checkSql,
        [=](const Json* json) {
            auto rows = (*json)["result_set"][0]["rows"];
            if (!rows.empty()) {
                sendJson(resp, "{\"msg\":\"exists\",\"id\":" + rows[0][0].dump() +
                               ",\"sha1\":\"" + sha1 +
                               "\",\"size\":" + std::to_string(size) + "}");
                return;
            }

            // 磁盘缺文件才写盘
            if (!fs::exists(absPath)) {
                fs::create_directories(fs::path(absPath).parent_path());
                std::string tmp = absPath + ".tmp." + FileService::uuid4();
                {
                    std::ofstream ofs(tmp, std::ios::binary);
                    ofs.write(content->data(), (std::streamsize)content->size());
                }
                std::error_code ec;
                fs::rename(tmp, absPath, ec);
            }

            std::string ins =
                "INSERT INTO file_meta(user_id,orig_name,sha1,category,size_bytes,storage_path) VALUES(" +
                std::to_string(uid) + ",'" + Db::escape(safeName) + "','" +
                sha1 + "','" + category + "'," + std::to_string(size) + ",'" + relPath + "')";

            resp->MySQL(Db::mysqlUrl(), ins, [=](const Json*) {
                resp->Redis(Db::redisUrl(), "DEL",
                    {"list:" + std::to_string(uid) + ":all",
                     "list:" + std::to_string(uid) + ":" + category},
                    [](const Json*){});
                sendJson(resp, "{\"msg\":\"ok\",\"sha1\":\"" + sha1 +
                               "\",\"size\":" + std::to_string(size) + "}");
            });
        });
}

// ---------------- 下载 ----------------
void FileController::download(const HttpReq* req, HttpResp* resp) {
    long uid = Auth::verify(req);
    if (uid < 0) { resp->set_status(HttpStatusUnauthorized); resp->String("unauthorized"); return; }

    std::string id  = req->param("id");
    std::string sql =
        "SELECT storage_path,orig_name FROM file_meta WHERE id=" + id +
        " AND user_id=" + std::to_string(uid) + " LIMIT 1";

    resp->MySQL(Db::mysqlUrl(), sql, [resp](const Json* json) {
        auto rows = (*json)["result_set"][0]["rows"];
        if (rows.empty()) { resp->set_status(HttpStatusNotFound); resp->String("not found"); return; }
        std::string rel  = rows[0][0].get<std::string>();
        std::string name = rows[0][1].get<std::string>();
        resp->add_header_pair("Content-Disposition",
                              "attachment; filename=\"" + name + "\"");
        resp->File(Config::storageRoot() + "/" + rel);
    });
}

// ---------------- 列表（Redis 缓存） ----------------
void FileController::list(const HttpReq* req, HttpResp* resp) {
    long uid = Auth::verify(req);
    if (uid < 0) { resp->set_status(HttpStatusUnauthorized); resp->String("unauthorized"); return; }

    std::string category = req->query("category");
    if (category.empty()) category = "all";
    std::string cacheKey = "list:" + std::to_string(uid) + ":" + category;

    resp->Redis(Db::redisUrl(), "GET", {cacheKey},
        [=](const Json* r) {
            if (r->contains("result") && (*r)["result"].is_string()) {
                sendJson(resp, (*r)["result"].get<std::string>());
                return;
            }
            std::string sql =
                "SELECT id,orig_name,category,size_bytes,sha1 FROM file_meta WHERE user_id=" +
                std::to_string(uid);
            if (category != "all")
                sql += " AND category='" + Db::escape(category) + "'";
            sql += " ORDER BY id DESC";

            resp->MySQL(Db::mysqlUrl(), sql, [=](const Json* j) {
                std::string body = (*j)["result_set"][0]["rows"].dump();
                resp->Redis(Db::redisUrl(), "SETEX",
                            {cacheKey, "60", body}, [](const Json*){});
                sendJson(resp, body);
            });
        });
}

// ---------------- 删除（引用归零删物理文件） ----------------
void FileController::remove(const HttpReq* req, HttpResp* resp) {
    long uid = Auth::verify(req);
    if (uid < 0) { resp->set_status(HttpStatusUnauthorized); resp->String("unauthorized"); return; }

    std::string id  = req->param("id");
    std::string sel =
        "SELECT sha1 FROM file_meta WHERE id=" + id +
        " AND user_id=" + std::to_string(uid) + " LIMIT 1";

    resp->MySQL(Db::mysqlUrl(), sel, [=](const Json* j) {
        auto rows = (*j)["result_set"][0]["rows"];
        if (rows.empty()) { resp->set_status(HttpStatusNotFound); resp->String("not found"); return; }
        std::string sha1 = rows[0][0].get<std::string>();

        std::string del =
            "DELETE FROM file_meta WHERE id=" + id +
            " AND user_id=" + std::to_string(uid);

        resp->MySQL(Db::mysqlUrl(), del, [=](const Json*) {
            std::string cnt = "SELECT COUNT(*) FROM file_meta WHERE sha1='" + sha1 + "'";
            resp->MySQL(Db::mysqlUrl(), cnt, [=](const Json* c) {
                long refs = (*c)["result_set"][0]["rows"][0][0].get<long>();
                if (refs == 0) {
                    std::error_code ec;
                    fs::remove(FileService::absolutePath(sha1), ec);
                }
                resp->Redis(Db::redisUrl(), "DEL",
                            {"list:" + std::to_string(uid) + ":all"}, [](const Json*){});
                sendJson(resp, "{\"msg\":\"deleted\"}");
            });
        });
    });
}

// ---------------- 秒传探测 ----------------
void FileController::checkSha1(const HttpReq* req, HttpResp* resp) {
    long uid = Auth::verify(req);
    if (uid < 0) { resp->set_status(HttpStatusUnauthorized); resp->String("unauthorized"); return; }

    std::string sha1 = req->query("sha1");
    if (sha1.size() != 40) { resp->String("bad sha1"); return; }

    std::string sql =
        "SELECT id FROM file_meta WHERE user_id=" + std::to_string(uid) +
        " AND sha1='" + sha1 + "' LIMIT 1";

    resp->MySQL(Db::mysqlUrl(), sql, [=](const Json* j) {
        auto rows = (*j)["result_set"][0]["rows"];
        if (rows.empty()) sendJson(resp, "{\"hit\":false}");
        else              sendJson(resp, "{\"hit\":true,\"id\":" + rows[0][0].dump() + "}");
    });
}
