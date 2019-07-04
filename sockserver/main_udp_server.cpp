//
// Created by tianjiajia on 7/2/19.
//
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>

int main() {

    /*Create socket*/
    int fd = socket(AF_INET, SOCK_DGRAM/*UDP*/, 0);
    if (fd == -1) {
        perror("Socket create failed: ");
        return EXIT_FAILURE;
    }

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
    /*int listen = ::listen(fd, 5);
    if (listen == -1) {
        perror("Listen failed: ");
        return EXIT_FAILURE;
    }

    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    int acc_fd = accept(fd, (sockaddr*)&client, &len);
    if (acc_fd == -1) {
        perror("Accept failed: ");
        return EXIT_FAILURE;
    }*/

    /*printf("Replay...\n\n");
    printf("Client info: port= %d\n", client.sin_port);

    const char* reply = "From server >>> Connect success";
    write(acc_fd, reply, strlen(reply));*/


    struct sockaddr_in client_sock;
    socklen_t client_addr_len;
    const int BUFF_SIZE = 1500;
    char buff[BUFF_SIZE];

    while(1) {
        printf("Ready to read...\n");
        ssize_t recv = ::recvfrom(fd, buff, BUFF_SIZE, 0, (sockaddr*)&client_sock, &client_addr_len);
        if (recv <= 0) {
            perror("err on recvfrom: ");
            return EXIT_FAILURE;
        }

        printf("received: %s\n", buff);

        memset(buff, 0, BUFF_SIZE);

        sleep(2);

        strcpy(buff, "write to clienttttttttttttt");
        ssize_t send = ::sendto(fd, buff, BUFF_SIZE, 0, (sockaddr*)&client_sock, client_addr_len);
        printf("send: %d\n", send);
    }

    close(fd);
    return 0;
}