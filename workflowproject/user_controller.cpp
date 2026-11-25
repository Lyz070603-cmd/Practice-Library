// user_controller.cpp
#include "user_controller.h"
#include "auth.h"
#include "db.h"
#include "config.h"
#include "file_service.h"
#include "http_util.h"
#include <wfrest/HttpContent.h>

using namespace wfrest;

void UserController::registerUser(const HttpReq* req, HttpResp* resp) {
    if (req->content_type() != APPLICATION_URLENCODED) {
        resp->File(Config::wwwRoot() + "/error.html");
        return;
    }
    auto form = req->form_kv();
    std::string username = Db::escape(form["username"]);
    std::string password = form["password"];
    if (username.empty() || password.empty()) {
        sendJson(resp, "{\"err\":\"empty\"}");
        return;
    }

    std::string salt = FileService::uuid4();
    std::string hash = Auth::sha256(password + salt);

    std::string sql =
        "INSERT INTO user(username,password,salt) VALUES('" +
        username + "','" + hash + "','" + salt + "')";

    resp->MySQL(Db::mysqlUrl(), sql, [resp](const Json*) {
        sendJson(resp, "{\"msg\":\"registered\"}");
    });
}

void UserController::login(const HttpReq* req, HttpResp* resp) {
    if (req->content_type() != APPLICATION_URLENCODED) {
        resp->File(Config::wwwRoot() + "/error.html");
        return;
    }
    auto form = req->form_kv();
    std::string username = Db::escape(form["username"]);
    std::string password = form["password"];
    if (username.empty() || password.empty()) {
        resp->File(Config::wwwRoot() + "/error.html");
        return;
    }

    std::string sql =
        "SELECT id,password,salt FROM user WHERE username='" + username + "' LIMIT 1";

    resp->MySQL(Db::mysqlUrl(), sql,
        [resp, username, password](const Json* json) {
            auto rows = (*json)["result_set"][0]["rows"];
            if (rows.empty()) { resp->File(Config::wwwRoot() + "/error.html"); return; }

            std::string stored = rows[0][1].get<std::string>();
            std::string salt   = rows[0][2].get<std::string>();
            if (Auth::sha256(password + salt) != stored) {
                resp->File(Config::wwwRoot() + "/error.html");
                return;
            }
            long uid = rows[0][0].get<long>();
            std::string token = Auth::generate(uid, username);
            sendJson(resp, "{\"token\":\"" + token +
                           "\",\"uid\":" + std::to_string(uid) + "}");
        });
}
