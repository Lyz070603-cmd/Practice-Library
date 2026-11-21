#include <workflow/WFFacilities.h>
#include <workflow/WFTaskFactory.h>
#include <workflow/HttpMessage.h>
#include <workflow/HttpUtil.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <iostream>

using std::cout;
using std::string;

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo) {
    (void)signo;
    wait_group.done();
    const char msg[] = "done\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);   // signal-safe
}

int main() {
    signal(SIGINT, sig_handler);

    auto *httptask = WFTaskFactory::create_http_task(
        "http://172.25.134.48:1280/",
        0, 0,
        [](WFHttpTask *task) {
            int state = task->get_state();
            int error = task->get_error();
            switch (state) {
                case WFT_STATE_SYS_ERROR:
                    fprintf(stderr, "system error: %s\n", strerror(error));
                    break;
                case WFT_STATE_DNS_ERROR:
                    fprintf(stderr, "DNS error: %s\n", gai_strerror(error));
                    break;
                case WFT_STATE_SSL_ERROR:
                    fprintf(stderr, "SSL error: %d\n", error);
                    break;
                case WFT_STATE_TASK_ERROR:
                    fprintf(stderr, "task error: %d\n", error);
                    break;
                case WFT_STATE_SUCCESS:
                    fprintf(stderr, "OK, status=%s\n",
                            task->get_resp()->get_status_code());
                    break;
            }
            protocol::HttpResponse *resp = task->get_resp();
            std::string name;
            std::string value;
            protocol::HttpHeaderCursor resp_cursor(resp);
            while (resp_cursor.next(name,value)){
                cout << "name: " << name << "value: " <<value <<"\n"; 
            }
            const void *body;
            size_t body_len;
            resp->get_parsed_body(&body,&body_len);
            fwrite(body,1,body_len,stdout);
            fflush(stdout);
            wait_group.done();         
        });

    protocol::HttpRequest *req = httptask->get_req();
    req->add_header_pair("Accept", "*/*");
    req->add_header_pair("User-Agent", "TestAgent");
    req->add_header_pair("Connection", "close");

    httptask->start();
    wait_group.wait();                  // 任务结束后正常返回
    return 0;
}
