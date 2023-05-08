/*
Assignment 03
S M Rafiuddin
CWID: A20387294

Acknowledgments:

Some code segments are used from-

Geeks For Geeks
*/



#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 1050
#define MAX_CONNECTIONS 3
#define MAX_BUFFER_SIZE 1024

pthread_mutex_t mutex;
char shared_file[1024] = {0};

void* handle_writer(void* arg) {
    int writer_sock = *(int*)arg;
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Read message from writer
    int valread = read(writer_sock, buffer, MAX_BUFFER_SIZE);
    printf("Received message from writer: %s\n", buffer);

    // Store message in the shared file (critical section)
    pthread_mutex_lock(&mutex);
    FILE* file = fopen("shared_file.txt", "a");
    fprintf(file, "%s\n", buffer);
    fclose(file);
    pthread_mutex_unlock(&mutex);
    printf("Message stored in shared file\n");

    // Wait for 2 seconds
    sleep(2);

    // Send contents of shared file back to the writer
    pthread_mutex_lock(&mutex);
    file = fopen("shared_file.txt", "r");
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    fread(shared_file, file_size, 1, file);
    fclose(file);
    pthread_mutex_unlock(&mutex);

    send(writer_sock, shared_file, strlen(shared_file), 0);
    printf("Contents of shared file sent back to the writer\n");

    close(writer_sock);
    printf("Connection with writer closed\n");

    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    pthread_t threads[MAX_CONNECTIONS];

    // Create shared file
    FILE* file = fopen("shared_file.txt", "w");
    fclose(file);
    printf("Shared file created\n");

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);
    printf("Mutex initialized\n");

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Server socket created\n");

    // Allow multiple connections on the same address
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    printf("Setsockopt completed\n");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the server socket to a specific address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }
    printf("Server socket bound to address and port\n");

    // Start listening for connections
    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening for connections\n");

    int connection_count = 0;
    while (connection_count < MAX_CONNECTIONS) {
        // Accept a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Acceptance failed");
            exit(EXIT_FAILURE);
        }

        // Create a new thread to handle the connection
        if (pthread_create(&threads[connection_count], NULL, handle_writer, &new_socket) != 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }

        connection_count++;
    }

    // Wait for all threads to finish
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Cleanup mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}

