#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstdlib>
#include <stddef.h>

#define SERV_SOCK "/tmp/service.socket"

int main() {
    /*Create server listen socket*/
    int listen_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    size_t len = strlen(SERV_SOCK);

    /*Create server local socket address.*/
    struct sockaddr_un sun;
    bzero(&sun, sizeof(sun));
    sun.sun_family = AF_UNIX;
    strcpy(sun.sun_path, SERV_SOCK);

    /*Binding address to a local socket file, bind will create socket file.*/
    unlink(SERV_SOCK); /*For ensure the bind operation will be success.*/
    size_t sockLen = offsetof(struct sockaddr_un, sun_path) + len;
    printf("sockLen: %d\n", sockLen);
    int ret = bind(listen_fd, (struct sockaddr*)&sun, (socklen_t)sockLen);
    printf("ret: %d\n", ret);
    if (ret != 0) {
        perror("err bind: ");
        return EXIT_FAILURE;
    }

    char buff[256];
    while (1) {
        ssize_t ret = recvfrom(listen_fd, buff, sizeof(buff), 0, NULL, 0);
        if (ret <= 0) {
            perror("recv err");
            return EXIT_FAILURE;
        }

        printf("serv received: %s\n", buff);

        int len = strlen(buff);
        for (int i = 0; i < len; ++i) {
            if (buff[i] >= 'a' || buff[i] <= 'z') {
                buff[i] -= 32;
            }
        }

        sendto(listen_fd, buff, strlen(buff), 0, (struct sockaddr*)&sun, sockLen);
        bzero(buff, sizeof(buff));
    }

    close(listen_fd);
    unlink(SERV_SOCK);

}

