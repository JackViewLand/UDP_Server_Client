#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 256

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);

    // create socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // set up server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    // bind socket to server address
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on %s:%d...\n", ip, port);

    // receive and echo back
    char buf[BUF_SIZE];
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    ssize_t recv_len;
    while (1) {
        memset(buf, 0, BUF_SIZE);
        recv_len = recvfrom(sock, buf, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (recv_len == -1) {
            perror("recvfrom");
            continue;
        }

        printf("Received message from %s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buf);

        if (sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&client_addr, addr_len) == -1) {
            perror("sendto");
        }
    }

    close(sock);
    return 0;
}



