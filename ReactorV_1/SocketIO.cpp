/**
 * Project Untitled
 */


#include "SocketIO.h"
#include <stddef.h>
#include <unistd.h> 
#include <errno.h>
#include <stdexcept>


/**
 * SocketIO implementation
 */


/**
 * @param fd
 */
SocketIO::SocketIO(int fd) 
:_fd(fd){

}

SocketIO::~SocketIO() {

}

/**
 * @param buf
 * @param len
 * @return void
 */
void SocketIO::read(char * buf, int len) {
     int left = len;
    int recvd = 0;

    while (left > 0) {
        ssize_t ret = ::read(_fd, buf + recvd, left);

        if (ret < 0) {
            if (errno == EINTR) {          // 被信号中断，重试
                continue;
            }
            throw std::runtime_error("read error");
        }

        if (ret == 0) {
            // 对端关闭连接：读到 0 就是 EOF
            throw std::runtime_error("read: peer closed");
        }

        recvd += ret;
        left  -= ret;
    }
    return;
}

/**
 * @param buf
 * @param len
 * @return void
 */
void SocketIO::write(const char * buf, int len) {
        int left = len;
    int written = 0;

    while (left > 0) {
        size_t ret = ::write(_fd, buf + written, left);   // 注意加 :: 调用系统函数

        if (ret < 0) {
            if (errno == EINTR) {          // 被信号中断，重试，不算错误
                continue;
            }
            throw std::runtime_error("write error");
            // 或者：perror("write"); return;
        }

        if (ret == 0) {
            // 对端异常关闭，写不进去了
            throw std::runtime_error("write: peer closed");
        }

        written += ret;
        left    -= ret;
    }
    return;
}
