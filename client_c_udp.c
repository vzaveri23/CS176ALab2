// Reference: https://stackoverflow.com/questions/35568996/socket-programming-udp-client-server-in-c
// Used the above link to set up the basic client and server code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Not enough arguments\n");
        exit(EXIT_FAILURE);
    }

    // Set up buffer and parse arguments from command line
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t len = sizeof(server_addr);
    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    const int BUFFER_SIZE = 129;
    char buffer[BUFFER_SIZE];
    memset(buffer, '\0', BUFFER_SIZE);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Assign address family, server port, and server IP
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);


    // Get user input and send string to server
    printf("Enter string: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *) &server_addr, len);


    // Receive message from server
    while (1) {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &len);
        if (n <= 0) {
            break;
        }
        
        // Insert null terminator at end of buffer
        buffer[n] = '\0';
        printf("From server: %s\n", buffer);

         // Check if the received message is a single digit or invalid input
         // Used chatGPT to determine error checking
        if (strcmp(buffer, "Sorry, cannot compute!") == 0 || 
            (strlen(buffer) == 1 && buffer[0] >= '0' && buffer[0] <= '9')) {
            break;
        }
    }
    
    close(sockfd);
    return 0;
}