#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

constexpr int PORT =2053;
costexpr int BUFFER_SIZE =512;

int main(){
    int sockfd;
    struct sockaddr_in servr_addr{}, client_addr{};
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // CREATE UDP SOCKET
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("socket creation failed");
        return -1;
    }

    // Bind to port 2053
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_Addr)) <0){
        perror("bind failed");
        close(sockfd);
        return -1;
    }
    while (true){
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,(struct sockaddr *)&client_addr, &client_len);
        if(n<0){
            perror("recvfrom failed");
            continue;
        }
        // BUILD DNS response header(12 bytes)
        uint8_t response[12] = {};

        // Transaction ID: 1234 
        response[0] = 0x04;
        response[1] = 0x00;

        response[2] = 0x00;
        response[3] = 0x00;
        
        response[4] = 0x00;
        response[5] = 0x00;

        response[6] = 0x00;
        response[7] = 0x00;

        response[8] = 0x00;
        response[9] = 0x00;

        response[10] = 0x00;
        response[11] = 0x00;

        sendto(sockfd, response, sizeof(response), 0, (struct sockaddr *)&client_addr, client_len);

    }
    close(sockfd);
    return 0;
}
