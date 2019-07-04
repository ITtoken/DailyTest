
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstdlib>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>

#define LOCAL_SOCK "/tmp/client.socket"
#define SERV_SOCK "/tmp/service.socket"

int main() {
    int connFd = socket(AF_UNIX, SOCK_STREAM, 0);
    size_t len = strlen(SERV_SOCK);

    //*Create local client socket files*//*
    struct sockaddr_un lun;
    bzero(&lun, sizeof(lun));
    lun.sun_family = AF_UNIX;
    strcpy(lun.sun_path, LOCAL_SOCK);

    /*Binding socket name*/
    unlink(LOCAL_SOCK);
    size_t addrLen = offsetof(struct sockaddr_un, sun_path) + len;
    int ret = bind(connFd, (struct sockaddr*)&lun, (socklen_t)addrLen);
    if (ret != 0) {
        perror("bind err: ");
        return EXIT_FAILURE;
    }

    /*Connect service socket.*/
    len = strlen(SERV_SOCK);
    struct sockaddr_un sun;
    bzero(&sun, sizeof(struct sockaddr_un));

    sun.sun_family = AF_UNIX;
    strcpy(sun.sun_path, SERV_SOCK);
    size_t serv_len = len + offsetof(struct sockaddr_un, sun_path);

    ret = ::connect(connFd, (struct sockaddr*)&sun, (socklen_t)serv_len);
    if (ret != 0) {
        printf("errno = %d\n", errno);
        perror("connect err: ");
        return EXIT_FAILURE;
    }

    /*Read and Write...*/
    char buff[1024];
    while (fgets(buff, sizeof(buff), stdin)) {
        send(connFd, buff, sizeof(buff), 0);
        bzero(buff, sizeof(buff));

        recv(connFd, buff, sizeof(buff), 0);
        printf("recv: %s\n", buff);

        bzero(buff, sizeof(buff));
    }
}

