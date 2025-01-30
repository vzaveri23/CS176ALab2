//inspiration for code from:
// https://stackoverflow.com/questions/35568996/socket-programming-udp-client-server-in-c, 
// https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
// and ChatGPT for syntactical decisions

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
    const int MAX_BUFFER_SIZE = 129;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));


    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        // close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        // close(server_fd);
        exit(EXIT_FAILURE);
    }


    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        memset(buffer, 0, MAX_BUFFER_SIZE);
        int bytes_read = read(client_fd, buffer, MAX_BUFFER_SIZE - 1);

        if (bytes_read <= 0) {
            perror("Read failed");
            close(client_fd);
            continue;
        }

        buffer[bytes_read] = '\0'; 

        if (!is_valid_input(buffer)) {
            const char *error = "Sorry, cannot compute!";
            write(client_fd, error, strlen(error));

        } else {
            char new_buffer[MAX_BUFFER_SIZE];
            strcpy(new_buffer, buffer);
            while (1) {
                int res = sum_of_digits(new_buffer);
                sprintf(new_buffer, "%d", res);
                write(client_fd, new_buffer, strlen(new_buffer));

                if (res < 10) break;
                usleep(1000);

            }
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
