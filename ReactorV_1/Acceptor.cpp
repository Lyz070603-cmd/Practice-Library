/**
 * Project Untitled
 */


#include "Acceptor.h"
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdexcept>

/**
 * Acceptor implementation
 */


/**
 * @param ip
 * @param port
 */
Acceptor::Acceptor(const string ip, const string port)
:_addr(ip,port){


}

/**
 * @param addr
 */
Acceptor::Acceptor(const InetAddress addr)
:_addr(addr){
    

}

Acceptor::~Acceptor() {

}

/**
 * @return int
 */
int Acceptor::accept() {
    return 0;
}

void Acceptor::ready(){
      bind();
    listen();

}
/**
 * @return void
 */
void Acceptor::bind() {
    if (::bind(_sockrt.getfd(),
               (struct sockaddr *)_addr.getaddr(),
               sizeof(struct sockaddr_in)) < 0) {
        throw std::runtime_error("Acceptor::bind failed");
    }
    return;
}

/**
 * @return void
 */
void Acceptor::listen() {
      if (::listen(_sockrt.getfd(),SOMAXCONN) < 0) {
        throw std::runtime_error("listen");
    }
    return;
}
