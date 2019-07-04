#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>

int main1() {

    /*Create socket*/
    int fd = socket(AF_INET, SOCK_STREAM, 0);
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
    int listen = ::listen(fd, 5);
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
    }

    printf("Replay...\n\n");
    printf("Client info: port= %d\n", client.sin_port);

    const char* reply = "From server >>> Connect success";
    write(acc_fd, reply, strlen(reply));

    char buff[1024];
    while(1) {
        printf("Ready to read...\n");
        int read = ::read(acc_fd, buff, sizeof(buff));
        if(read > 0) {
            printf("read OK: %s\n", buff);
        } else{
            printf("read %d\n", read);
        }

        write(acc_fd, reply, strlen(reply));
        printf("write ok\n");
    }

    close(acc_fd);
    close(fd);


    return 0;
}