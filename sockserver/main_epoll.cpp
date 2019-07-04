//
// Created by tianjiajia on 6/28/19.
//
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/epoll.h>

int main1() {

    /*Create socket*/
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("Socket create failed: ");
        return EXIT_FAILURE;
    }

    printf("listen listen_fd= %d\n", listen_fd);

    /*Bind addr.*/
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5002); //port
    addr.sin_addr.s_addr = htonl(INADDR_ANY); //ip
    int bind = ::bind(listen_fd, (sockaddr*)&addr, sizeof(addr));
    if (bind == -1) {
        perror("Bind failed: ");
        return EXIT_FAILURE;
    }

    /*Set listen.*/
    int listen = ::listen(listen_fd, 5);
    if (listen == -1) {
        perror("Listen failed: ");
        return EXIT_FAILURE;
    }

    int epollFD = epoll_create(10);

    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = listen_fd;
    int ret = epoll_ctl(epollFD, EPOLL_CTL_ADD, listen_fd, &event);
    if (ret == -1) {
        perror("epoll_ctl error: ");
        return EXIT_FAILURE;
    }

    while (1) {
        epoll_event out_event[10];
        int nfds = epoll_wait(epollFD, out_event, 10, -1);
        if (nfds == -1) {
            perror("epoll_ctl error: ");
            return EXIT_FAILURE;
        }

        printf("nfds= %d\n", nfds);

        for (int i = 0; i < nfds; ++i) {
            int events = out_event[i].events;
            int efd = out_event[i].data.fd;

            if (events & EPOLLIN) {
                if (efd == listen_fd) {
                    /*do accept*/
                    struct sockaddr_in client;
                    socklen_t len = sizeof(client);
                    int acc_fd = accept(listen_fd, (sockaddr*)&client, &len);
                    printf("accept listen_fd= %d\n", acc_fd);
                    if (acc_fd == -1) {
                        perror("Accept failed: ");
                        return EXIT_FAILURE;
                    }

                    epoll_event e;
                    e.events = EPOLLIN;
                    e.data.fd = acc_fd;
                    epoll_ctl(epollFD, EPOLL_CTL_ADD, acc_fd, &e);
                    printf("Add fd OK\n");

                    const char* reply = "Server reply....";

                    write(acc_fd, reply, strlen(reply));

                    if (nfds == 1) {
                        continue;
                    }
                } else {
                    //other fd.

                    char buff[1024];
                    printf("Ready to read...\n");
                    int read = ::read(efd, buff, sizeof(buff));
                    if(read > 0) {
                        printf("read OK: %s\n", buff);
                    } else{
                        printf("read %d\n", read);
                        close(efd);
                        printf("closed: %d\n", efd);
                        continue;
                    }

                    const char* reply = "Server reply....";

                    write(efd, reply, strlen(reply));
                    printf("write ok\n");
                }
            }
        }
    }

    close(listen_fd);


    return 0;
}

