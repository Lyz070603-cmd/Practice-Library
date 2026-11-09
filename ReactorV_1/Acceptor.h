/**
 * Project Untitled
 */


#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

class Acceptor {
public: 
    
/**
 * @param ip
 * @param port
 */
void Acceptor(const string ip, const string port);
    
/**
 * @param addr
 */
void Acceptor(const InetAddress addr);
    
void ~Acceptor();
    
int accept();
private: 
    Socket _sockrt;
    InetAddress _addr;
    
void bind();
    
void listen();
};

#endif //_ACCEPTOR_H