/**
 * Project Untitled
 */


#include "EventLoop.h"
#include <sys/epoll.h>

/**
 * EventLoop implementation
 */


/**
 * @param acceptor
 */
EventLoop::EventLoop(Acceptor & acceptor) 
:_epfd(creatEpollFd())
,_isLoop(false)
,_accept(acceptor){

    //添加listenfd
    
    addEpollReadFd(_accept.getlfd());

}

EventLoop::~EventLoop() {

}

/**
 * @return void
 */
void EventLoop::Loop() {
    _isLoop = true;
    while(true){
        waitEpollFd();
    }
    return;
}

/**
 * @return void
 */
void EventLoop::unLoop() {
    _isLoop = false;
    return;
}

/**
 * @param cd
 * @return void
 */
void EventLoop::setNewConnection(Tcpconnection && cd) {
    return;
}

/**
 * @param cd
 * @return void
 */
void EventLoop::setMessage(Tcpconnection && cd) {
    return;
}

/**
 * @param cd
 * @return void
 */
void EventLoop::setClosed(Tcpconnection && cd) {
    return;
}

/**
 * @return void
 */
void EventLoop::waitEpollFd() {
    int ret = epoll_wait(_epfd,&_eventlist,1024,3000);
    for (int i = 0 ; i < ret ; ++i){
        if (_eventlist[i] == _epfd){
            //有新的链接
            int nfd = _accept.accept();  

            addEpollReadFd(nfd);
            
            TcpconnectionPtr tcpnet;

            tcpnet->set
            _conns[nfd,]

        }
        else{
            //处理旧连接
        }
    }

    return;
}

/**
 * @return void
*/
int EventLoop::creatEpollFd() {
    _epfd = epoll_create1(0);
    return _epfd;
}

/**
 * @param fd
 * @return void
 */
void EventLoop::addEpollReadFd(int fd) {
    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;

    _eventlist.

    epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&event);
    return;
}

/**
 * @param fd
 * @return void
 */
void EventLoop::delEpollReadFd(int fd) {
    if (epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,nullptr) == -1){
        printf("delEpollReadFd error\n");
        return ;
    }
    
    return;
}

/**
 * @return void
 */
void EventLoop::handleNewConnection() {
    return;
}

/**
 * @param fd
 * @return void
 */
void EventLoop::handleMessage(int fd) {
    return;
}
