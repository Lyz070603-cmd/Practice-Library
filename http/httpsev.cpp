#include <workflow/WFHttpServer.h>
#include <workflow/HttpMessage.h>
#include <workflow/WFFacilities.h>
#include <workflow/HttpUtil.h>
#include <iostream>
#include <signal.h>
#include <string>


using std::cout;
using std::endl;
using std::string;

static WFFacilities::WaitGroup wait_group(1);
void sighandler(int signo){
    wait_group.done();
    cout << "sever stop" << endl;
    return ;
}

void process(WFHttpTask *task){
   task->get_resp()->append_output_body("Hello form sever!\n"); 
}
int main (){
    signal(SIGINT,sighandler);
    WFHttpServer sever(process);
    if (sever.start(1234) == 0){
        wait_group.wait();
        sever.stop();
    }
    else {
        perror("sever start error!\n");
        exit(-1);
    }

    return 0;
}
