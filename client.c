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

    // 2. 连接服务端
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 假设服务端在本机
    serAddr.sin_port = htons(PORT);

    if (connect(sfd, (struct sockaddr*)&serAddr, sizeof(serAddr)) == -1) {
        perror("connect");
        close(sfd);
        exit(1);
    }
    printf("已连接到服务端！你可以开始输入消息聊天了：\n");

    // 3. 使用 select 监听标准输入和网络套接字
    fd_set rdset;
    fd_set needMonitorSet;
    FD_ZERO(&rdset);
    FD_ZERO(&needMonitorSet);

    FD_SET(STDIN_FILENO, &needMonitorSet);
    FD_SET(sfd, &needMonitorSet);

    char buf[BUF_SIZE] = {0};
    int maxfd = sfd + 1;

    while (1) {
        memcpy(&rdset, &needMonitorSet, sizeof(fd_set));

        int readyNum = select(maxfd, &rdset, NULL, NULL, NULL);
        if (readyNum == -1) {
            perror("select");
            break;
        }

        // 键盘输入就绪（你打字了）
        if (FD_ISSET(STDIN_FILENO, &rdset)) {
            memset(buf, 0, sizeof(buf));
            read(STDIN_FILENO, buf, sizeof(buf) - 1);
            send(sfd, buf, strlen(buf), 0);
        }

        // 网络数据就绪（服务端发消息来了）
        if (FD_ISSET(sfd, &rdset)) {
            memset(buf, 0, sizeof(buf));
            int ret = recv(sfd, buf, sizeof(buf) - 1, 0);
            if (ret == 0) {
                printf("\n服务端已断开连接。\n");
                break;
            } else if (ret > 0) {
                printf("\n[服务端发来消息]: %s", buf);
            }
        }
    }

    close(sfd);
    return 0;
}
