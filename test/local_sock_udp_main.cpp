
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstdlib>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>

#define CLI_SOCK "/tmp/client.socket"
#define SERV_SOCK "/tmp/service.socket"

int main() {
    int connFd = socket(AF_UNIX, SOCK_DGRAM, 0);

    struct sockaddr_un client_un, server_un;
    bzero(&client_un, sizeof(struct sockaddr_un));
    bzero(&server_un, sizeof(struct sockaddr_un));

    size_t len = strlen(CLI_SOCK);
    /*Client config*/
    client_un.sun_family = AF_UNIX;
    strcpy(client_un.sun_path, CLI_SOCK);
    size_t cli_len = len + offsetof(struct sockaddr_un, sun_path);
    unlink(CLI_SOCK);
    int ret = bind(connFd, (struct sockaddr*)&client_un, (socklen_t)cli_len);
    if (ret != 0) {
        perror("bind err: ");
        return EXIT_FAILURE;
    }

    /*Server config*/
    len = strlen(SERV_SOCK);
    server_un.sun_family = AF_UNIX;
    strcpy(server_un.sun_path, SERV_SOCK);
    size_t serv_len = len + offsetof(struct sockaddr_un, sun_path);

    struct sockaddr_un targetUn;
    bzero(&targetUn, sizeof(targetUn));
    int targetLen;

    /*Read and Write...*/
    char buff[256];
    while (fgets(buff, sizeof(buff), stdin)) {
        ssize_t send = sendto(connFd, buff, strlen(buff), 0, (struct sockaddr*)&server_un, (socklen_t)serv_len);
        if (send <= 0) {
            perror("send err");
            return EXIT_FAILURE;
        }

        printf("send: %d\n", send);

        bzero(buff, sizeof(buff));

        ssize_t recv = recvfrom(connFd, buff, sizeof(buff), 0, NULL, 0);
        if (recv <= 0) {
            perror("recv err");
            return EXIT_FAILURE;
        }
        printf("recv: %s\n", buff);

        bzero(buff, sizeof(buff));


    }
}

