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
    
/**
 * @param fd &
 */
void getfd(int & fd );
private: 
    int _fd;
};

#endif //_SOCKET_H
