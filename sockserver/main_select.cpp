#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/select.h>

int main1() {

    /*Create socket*/
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("Socket create failed: ");
        return EXIT_FAILURE;
    }

    printf("listen fd= %d\n", fd);

    /*Bind addr.*/
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5002); //port
    addr.sin_addr.s_addr = htonl(INADDR_ANY); //ip
    int bind = ::bind(fd, (sockaddr*)&addr, sizeof(addr));
    if (bind == -1) {
        perror("Bind failed: ");
        return EXIT_FAILURE;
    }

    /*Set listen.*/
    int listen = ::listen(fd, 5);
    if (listen == -1) {
        perror("Listen failed: ");
        return EXIT_FAILURE;
    }


    int maxFd = fd;
    fd_set rset, allset;
    FD_ZERO(&allset);

    FD_SET(fd/*listen fd*/, &allset);

    while (1) {
        rset = allset;

        int ret = select(maxFd + 1, &rset, NULL, NULL, NULL);
        if (ret > 0) {
            printf("incoming: %d\n", ret);
            for(int i = fd; i < maxFd; ++i) {
                printf("ddd: %d = %d\n", i, FD_ISSET(i, &rset));
            }
            if (FD_ISSET(fd, &rset)) {

                /*do accept*/
                struct sockaddr_in client;
                socklen_t len = sizeof(client);
                int acc_fd = accept(fd, (sockaddr*)&client, &len);
                printf("accept fd= %d", acc_fd);
                if (acc_fd == -1) {
                    perror("Accept failed: ");
                    return EXIT_FAILURE;
                }



                if (maxFd < acc_fd) {
                    maxFd = acc_fd;
                }

                printf("add new fd to select listening... %d\n", acc_fd);
                FD_SET(acc_fd, &allset);
                if (ret == 1) {
                    continue;
                }
            }
            for (int i = fd + 1/*exclude the listen fd*/; i <= maxFd; ++i) {
                if(FD_ISSET(i, &rset)) {
                    char buff[1024];
                    printf("Ready to read...\n");
                    int read = ::read(i, buff, sizeof(buff));
                    if(read > 0) {
                        printf("read OK: %s\n", buff);
                    } else{
                        printf("read %d\n", read);
                        close(i);
                        FD_CLR(i, &allset);
                        printf("closed: %d\n", i);
                        continue;
                    }

                    const char* reply = "Server reply....";

                    write(i, reply, strlen(reply));
                    printf("write ok\n");
                }
            }
        }
    }

    close(fd);


    return 0;
}