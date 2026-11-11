/**
 * Project Untitled
 */


#include "InetAddress.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
/**
 * InetAddress implementation
 */


/**
 * @param ip
 * @param port
 */
InetAddress::InetAddress(const string ip, const string port) {
    _add.sin_family = AF_INET;
    _add.sin_port = htons(atoi(port.c_str()));
    _add.sin_family = inet_addr(ip.c_str());
}

/**
 * @param rhs
 */
InetAddress::InetAddress(const InetAddress & rhs) {
    _add = rhs._add;
}

InetAddress::~InetAddress() {

}

/**
 * @return struct sockaddr_in *
 */
struct sockaddr_in * InetAddress::getaddr() {

    return &_add;
}

/**
 * @return string
 */
string InetAddress::Ip() {
    return std::to_string(ntohs(_add.sin_port));
}

/**
 * @return string
 */
string InetAddress::Port() {
    return inet_ntoa(_add.sin_addr);
}
