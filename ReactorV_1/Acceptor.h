/**
 * Project Untitled
 */


#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H
#include <string>
#include "InetAddress.h"
#include "Socket.h"
using std::string;

class Acceptor {
public: 
    
/**
 * @param ip
 * @param port
 */
Acceptor(const string ip, const string port);
    
/**
 * @param addr
 */
Acceptor(const InetAddress addr);
    
~Acceptor();
    
int accept();

void ready();
private: 
    Socket _sockrt;
    InetAddress _addr;
    
void bind();
    
void listen();
};

#endif //_ACCEPTOR_H
