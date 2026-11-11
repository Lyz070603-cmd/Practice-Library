/**
 * Project Untitled
 */


#include "Socket.h"
#include <sys/socket.h>
#include <unistd.h>

/**
 * Socket implementation
 */


Socket::Socket() {
    _fd = socket(AF_INET,SOCK_STREAM,0);
}

/**
 * @param fd
 */
Socket::Socket(int fd) {
    _fd = fd;
}


Socket::~Socket() {
    close(_fd);

}

void Socket::setFd(const int & fd ){
    _fd = fd;
}
/**
 * @param fd &
 * @return void
 */
int Socket::getfd() {
    return _fd;
}
