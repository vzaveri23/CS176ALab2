//inspiration for code from:
// https://stackoverflow.com/questions/35568996/socket-programming-udp-client-server-in-c, 
// https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
// and ChatGPT for syntactical decisions, error checking

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


int main(int argc, char *argv[]) {
    const int MAX_BUFFER_SIZE = 129;
    if (argc != 3) {
        // fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *ip_addr = argv[1];
    int port = atoi(argv[2]);
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&ip_addr, 0, sizeof(ip_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Enter string: ");
    fgets(buffer, MAX_BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    write(sock_fd, buffer, strlen(buffer));

    while (1) {
        memset(buffer, 0, MAX_BUFFER_SIZE);
        int bytes_read = read(sock_fd, buffer, MAX_BUFFER_SIZE - 1);
        if (bytes_read <= 0) break;
        buffer[bytes_read] = '\0';
        printf("From server: %s\n", buffer);

        if (strcmp(buffer, "Sorry, cannot compute!") == 0 || 
            (strlen(buffer) == 1 && buffer[0] >= '0' && buffer[0] <= '9')) {
            break;
        }
    }

    close(sock_fd);
    return 0;
}
