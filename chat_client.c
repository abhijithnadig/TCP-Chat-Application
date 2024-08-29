#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Structure to represent a chat message
typedef struct {
    char message[BUFFER_SIZE];
} chat_message_t;

int main() {
    // Create client socket
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket creation failed");
        exit(1);
    }

    // Set address and port number for the server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // Connect to server
    if (connect(client_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        exit(1);
    }

    printf("Connected to server. Type 'exit' to quit.\n");

    while (1) {
        // Get user input
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Check if user wants to exit
        if (strcmp(buffer, "exit\n") == 0) {
            break;
        }

        // Send message to server
        send(client_fd, buffer, strlen(buffer), 0);

        // Receive response from server
        recv(client_fd, buffer, BUFFER_SIZE, 0);
        printf("Server response: %s\n", buffer);
    }

    // Close client socket
    close(client_fd);

    return 0;
}