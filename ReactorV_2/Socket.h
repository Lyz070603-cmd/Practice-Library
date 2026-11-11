/**
 * Project Untitled
 */


#ifndef _SOCKET_H
#define _SOCKET_H

class Socket {
public: 
    
Socket();
    
/**
 * @param fd
 */
explicit Socket(int fd);
    
~Socket();

void setFd(const int & fd);
    
/**
 * @param fd &
 */
int getfd();
private: 
    int _fd;
};

#endif //_SOCKET_H
