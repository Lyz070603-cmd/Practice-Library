/**
 * Project Untitled
 */


#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

class Tcpconnection {
public: 
    
/**
 * @param fd
 */
void Tcpconnection(int fd);
    
void ~Tcpconnection();
    
/**
 * @param msg
 */
void send(const string & msg);
    
string receive();
private: 
    SocketIO _sockfd;
};

#endif //_TCPCONNECTION_H