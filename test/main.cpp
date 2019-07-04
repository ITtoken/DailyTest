#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <cstdlib>

int main() {

    //1. Create socket
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("Create socket failed: ");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5002) ;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    socklen_t addrlen;
    addrlen = sizeof(server_addr);

    /*int dest;
    inet_pton(AF_INET,"192.168.1.108", (void*)&dest);
    addr.sin_addr.s_addr = static_cast<uint32_t >(dest);*/

    /*int ret = connect(fd, (sockaddr*)&addr, sizeof(addr));
    if (ret == -1) {
        perror("Connect failed: ");
        return EXIT_FAILURE;
    }

    const char* reply = "Client read OK!!!";*/
    socklen_t server_addr_len;
    const int BUFFER_SIZE = 1500;
    char buf[BUFFER_SIZE];

    while (1) {
        strcpy(buf, "client send...");
        ssize_t  send = ::sendto(fd, buf, BUFFER_SIZE, 0, (sockaddr*)&server_addr, sizeof(server_addr));
        printf("slient send: %d\n", send);

        ssize_t  recv = ::recvfrom(fd, buf, BUFFER_SIZE, 0, (sockaddr*)&server_addr, &server_addr_len);
        if (recv <= 0) {
            perror("recv err: ");
            return EXIT_FAILURE;
        }

        printf("receive: %s, len: %d\n", buf, recv);

        memset(buf, 0, BUFFER_SIZE);
        sleep(2);
    }

    sleep(2);
    close(fd);

    return 0;
}