#include <vector>
#include <workflow/WFTaskFactory.h>
#include <workflow/WFFacilities.h>
#include <workflow/Workflow.h>
#include <workflow/HttpMessage.h>
#include <string>
#include <signal.h>
#include <iostream>
#include <stdlib.h>

using std::string;
using std::vector;

struct SeriesContext{
    string name;
    size_t size;
};
static WFFacilities::WaitGroup wait_group(1);
void handler(int signo){
    wait_group.done();
    return ;
}
void httpcallback(WFHttpTask * httptask){
    protocol::HttpResponse * resp = httptask->get_resp();
    const void *body = nullptr;
    size_t size;

    SeriesWork *series = series_of(httptask);
    SeriesContext *ctx = static_cast<SeriesContext *>(series->get_context());
    if (!ctx) return;
    resp->get_parsed_body(&body,&size);
    ctx->size = size;
    
    printf("%s : size : %ld\n",ctx->name.c_str(),ctx->size);
    write(STDOUT_FILENO,body,size);


}
int main (){
    signal(SIGINT,handler);
    ParallelWork *work = Workflow::create_parallel_work(nullptr);

    vector<string> vec{
        "http://www.qq.com",
        "http://www.baidu.com",
        "http://127.0.0.1:1234/"
    };
    for (int i = 0 ; i < 3 ;++i){
        WFHttpTask *httptask = WFTaskFactory::create_http_task(vec[i],
                                                               10,0,httpcallback);

        SeriesWork * series = Workflow::create_series_work(httptask,nullptr);

        SeriesContext * context = new SeriesContext;
        series->set_context(context);
        context->name = vec[i];

        work->add_series(series);
    }

    work->start();
    wait_group.wait();
    return 0;
}
