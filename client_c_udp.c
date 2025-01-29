#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h> 

#define BUFFER_SIZE 129

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Not enough arguments\n");
        exit(EXIT_FAILURE);
    }


    int sockfd;
    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    char buffer[BUFFER_SIZE];

    if (sockfd = socket(AF_INET, SOCK_DGRAM, 0) < 0) {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter string: ");
    scanf("%s", buffer);
    for (int i = 0; i < strlen(buffer); i++) {
        printf("%c", buffer[i]);
    }

    return 0;
}