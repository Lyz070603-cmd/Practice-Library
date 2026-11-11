/**
 * Project Untitled
 */


#ifndef _INETADDRESS_H
#define _INETADDRESS_H
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
using std::string;


class InetAddress {
public: 
    
/**
 * @param ip
 * @param port
 */
InetAddress(const string ip, const string port);
    
/**
 * @param rhs
 */
InetAddress(const InetAddress &rhs);
    
~InetAddress();
    
struct sockaddr_in * getaddr();
    
string Ip();
    
string Port();
private: 
    struct sockaddr_in _add;
};

#endif //_INETADDRESS_H
