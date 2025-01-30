#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

int is_valid_input(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }

    return 1;
}

int sum_of_digits(const char *str) {
    int res = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        res += str[i] - '0';
    }

    return res;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Not enough arguments\n");
        exit(EXIT_FAILURE);
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr, client_addr;
    const int BUFFER_SIZE = 129;
    char buffer[BUFFER_SIZE];
    memset(buffer, '\0', BUFFER_SIZE);
    socklen_t len = sizeof(client_addr);
    int port = atoi(argv[1]);

    // Create socket
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket with the server address
    int bindStatus = bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bindStatus < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        memset(buffer, '\0', BUFFER_SIZE);
        // Receive message from client
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &len);
        buffer[n] = '\0';

        if (!is_valid_input(buffer)) {
            const char *return_val = "Sorry, cannot compute!";
            sendto(sockfd, return_val, strlen(return_val), 0, (const struct sockaddr *)&client_addr, len);
            continue;
        } else {
            char new_buffer[BUFFER_SIZE];
            strcpy(new_buffer, buffer);
            while (1) {
                int res = sum_of_digits(new_buffer);
                sprintf(new_buffer, "%d", res);
                sendto(sockfd, new_buffer, strlen(new_buffer), 0, (const struct sockaddr *)&client_addr, len);

                if (res < 10) break;
            }
        }

        
    }
    
    close(sockfd);
    return 0;
}