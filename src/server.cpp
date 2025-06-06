#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

constexpr int PORT = 2053;
constexpr int BUFFER_SIZE = 512;

int main() {
    int sockfd;
    struct sockaddr_in server_addr{}, client_addr{};
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sockfd);
        return 1;
    }

    while (true) {
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                             (struct sockaddr *)&client_addr, &client_len);
        if (n < 0) {
            perror("recvfrom failed");
            continue;
        }

        //DNS response header (12 bytes)
        uint8_t response[BUFFER_SIZE] = {};
        size_t offset = 0;

        response[offset++] = 0x04;
        response[offset++] = 0xD2;

        response[offset++] = 0x80;
        response[offset++] = 0x00;

        response[offset++] = 0x00;
        response[offset++] = 0x01;

        response[offset++] = 0x00;
        response[offset++] = 0x00;

        response[offset++] = 0x00;
        response[offset++] = 0x00;

        response[offset++] = 0x00;
        response[offset++] = 0x00;

        const uint8_t qname[] = {
            0x0c, 'c','o','d','e','c','r','a','f','t','e','r','s',
            0x02, 'i','o',
            0x00
        };
        memcpy(&response[offset], qname, sizeof(qname));
        offset += sizeof(qname);

        response[offset++] = 0x00;
        response[offset++] = 0x01;

        response[offset++] = 0x00;
        response[offset++] = 0x01;

        sendto(sockfd, response, offset, 0,
               (struct sockaddr *)&client_addr, client_len);
    }

    close(sockfd);
    return 0;
}
