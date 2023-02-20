#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <math.h>

#define BUF_SIZE 1024
#define MAX_RETRIES 10
#define BASE_BACKOFF 500
#define MAX_BACKOFF 8000
#define BACKOFF_MULTIPLIER 2

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <IP> <port> <message>\n", argv[0]);
        exit(1);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    char *message = argv[3];

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket error");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    int num_retries = 0;
    int backoff_time = BASE_BACKOFF;

    while (num_retries < MAX_RETRIES) {
        if (sendto(sock, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
            perror("sendto error");
            exit(1);
        }

        printf("Sent message: %s\n", message);

        char buf[BUF_SIZE];
        memset(buf, 0, BUF_SIZE);

        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);

        struct timeval tv;
        tv.tv_sec = backoff_time / 1000;
        tv.tv_usec = (backoff_time % 1000) * 1000;

        if (select(sock + 1, &readfds, NULL, NULL, &tv) == -1) {
            perror("select error");
            exit(1);
        }

        if (FD_ISSET(sock, &readfds)) {
            if (recvfrom(sock, buf, BUF_SIZE, 0, (struct sockaddr*)&client_addr, &addr_len) == -1) {
                perror("recvfrom error");
                exit(1);
            }

            printf("Received message: %s\n", buf);
            exit(0);
        } else {
            printf("Timeout: retrying...\n");
            num_retries++;
            backoff_time = fmin(backoff_time * BACKOFF_MULTIPLIER, MAX_BACKOFF);
        }
    }

    printf("Max retries exceeded\n");
    exit(1);
}




