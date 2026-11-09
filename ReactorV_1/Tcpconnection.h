/**
 * Project Untitled
 */


#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H
#include <string>
#include "SocketIO.h"
using std::string;

class Tcpconnection {
public: 
    
/**
 * @param fd
 */
Tcpconnection(int fd);
    
~Tcpconnection();
    
/**
 * @param msg
 */
void send(string & msg);
    
string receive();
private: 
    SocketIO _sockfd;
};

#endif //_TCPCONNECTION_H
