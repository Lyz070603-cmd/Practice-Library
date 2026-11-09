/**
 * Project Untitled
 */


#ifndef _INETADDRESS_H
#define _INETADDRESS_H

class InetAddress {
public: 
    
/**
 * @param ip
 * @param port
 */
void InetAddress(const string ip, const string port);
    
/**
 * @param rhs
 */
void InetAddress(const InetAddress rhs);
    
void ~InerAddress();
    
struct sockaddr_in * getaddr();
    
string Ip();
    
string Port();
private: 
    struct sockaddr_int _add;
};

#endif //_INETADDRESS_H