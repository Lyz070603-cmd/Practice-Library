/**
 * Project Untitled
 */


#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H
#include "Tcpconnection.h"
#include "Acceptor.h"
#include <vector>
#include <memory>
#include <map>
#include <functional>

using namespace std;

using TcpconnectionPtr = shared_ptr<Tcpconnection>;
using TcpconnectionCallBack =  function<void(const TcpconnectionPtr &)>;

class EventLoop {
public: 
    
/**
 * @param acceptor
 */
EventLoop(Acceptor & acceptor);
    
~EventLoop();
    
void Loop();
    
void unLoop();
    
/**
 * @param cd
 */
void setNewConnection(Tcpconnection && cd);
    
/**
 * @param cd
 */
void setMessage(Tcpconnection && cd);
    
/**
 * @param cd
 */
void setClosed(Tcpconnection && cd);
private: 
    int _epfd;
    bool _isLoop;
    vector<struct epoll_event> _eventlist;
    Acceptor & _accept;
    map<int,shared_ptr<Tcpconnection>> _conns;
    TcpconnectionCallBack _onNewConnectionCb;
    TcpconnectionCallBack _onMessageCb;
    TcpconnectionCallBack _onClosedCb;
    
void waitEpollFd();
    
int creatEpollFd();
    
/**
 * @param fd
 */
void addEpollReadFd(int fd);
    
/**
 * @param fd
 */
void delEpollReadFd(int fd);
    
void handleNewConnection();
    
/**
 * @param fd
 */
void handleMessage(int fd);
};

#endif //_EVENTLOOP_H
