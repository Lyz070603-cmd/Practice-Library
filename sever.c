#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8888
#define BUF_SIZE 1024

int main() {
    // 1. 创建 TCP 套接字
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
        perror("socket");
        exit(1);
    }

    // 设置端口复用（防止重启时端口被占用）
    int opt = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. 绑定本机 IP 和端口
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 监听所有网卡
    serAddr.sin_port = htons(PORT);

    if (bind(sfd, (struct sockaddr*)&serAddr, sizeof(serAddr)) == -1) {
        perror("bind");
        close(sfd);
        exit(1);
    }

    // 3. 监听端口
    if (listen(sfd, 10) == -1) {
        perror("listen");
        close(sfd);
        exit(1);
    }
    printf("服务端已启动，等待客户端连接...\n");

    // 4. 接收连接
    struct sockaddr_in cliAddr;
    socklen_t cliLen = sizeof(cliAddr);
    int newFd = accept(sfd, (struct sockaddr*)&cliAddr, &cliLen);
    if (newFd == -1) {
        perror("accept");
        close(sfd);
        exit(1);
    }
    printf("客户端已连接，IP: %s, Port: %d\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
    printf("你可以开始输入消息聊天了：\n");

    // 5. 使用 select 实现 I/O 多路复用
    fd_set rdset;       // 本次就绪的集合
    fd_set needMonitorSet; // 需要一直监控的集合
    FD_ZERO(&rdset);
    FD_ZERO(&needMonitorSet);

    // 将标准输入、监听套接字、客户端套接字加入监控集合
    FD_SET(STDIN_FILENO, &needMonitorSet);
    FD_SET(sfd, &needMonitorSet);
    FD_SET(newFd, &needMonitorSet);

    char buf[BUF_SIZE] = {0};
    int maxfd = newFd + 1; // select 的第一个参数是最大 fd + 1

    while (1) {
        // 每次循环必须重置 rdset，因为 select 会修改集合内容
        memcpy(&rdset, &needMonitorSet, sizeof(fd_set));

        int readyNum = select(maxfd, &rdset, NULL, NULL, NULL);
        if (readyNum == -1) {
            perror("select");
            break;
        }

        // 判断标准输入是否有数据（服务端打字）
        if (FD_ISSET(STDIN_FILENO, &rdset)) {
            memset(buf, 0, sizeof(buf));
            read(STDIN_FILENO, buf, sizeof(buf) - 1);
            send(newFd, buf, strlen(buf), 0);
        }

        // 判断客户端是否发来消息
        if (FD_ISSET(newFd, &rdset)) {
            memset(buf, 0, sizeof(buf));
            int ret = recv(newFd, buf, sizeof(buf) - 1, 0);
            if (ret == 0) {
                printf("\n客户端已断开连接。\n");
                FD_CLR(newFd, &needMonitorSet);
                close(newFd);
                break; // 退出循环
            } else if (ret > 0) {
                printf("\n[客户端发来消息]: %s", buf);
            }
        }
    }

    close(sfd);
    return 0;
}
