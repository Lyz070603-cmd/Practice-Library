/**
 * Project Untitled
 */


#include "Tcpconnection.h"

/**
 * Tcpconnection implementation
 */


/**
 * @param fd
 */
Tcpconnection::Tcpconnection(int fd)
:_sockfd(fd){

}

Tcpconnection::~Tcpconnection() {

}

/**
 * @param msg
 * @return void
 */
void Tcpconnection::send(string & msg) {
    _sockfd.write(msg.c_str(),msg.size());
    return;
}

/**
 * @return string
 */
string Tcpconnection::receive() {
    char buf[1500] = {0};
    _sockfd.read(buf,1500);
    printf("%s\n",buf);
    return "";
}
