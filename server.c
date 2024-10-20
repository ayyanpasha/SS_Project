// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "handler.h" 
#include "customer.h"
#include <sys/mman.h>
#include <sys/shm.h>
#include "server.h"


#define PORT 8080

char (*shared_usernames)[50];


void initialize_shared_memory() {
    // Key for the shared memory segment
    key_t key = ftok("shmfile", 65);

    // Create shared memory segment
    int shmid = shmget(key, MAX_USERS * 50 * sizeof(char), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        return;
    }

    // Attach shared memory
    shared_usernames = shmat(shmid, NULL, 0);
    if (shared_usernames == (void*)-1) {
        perror("shmat failed");
        return;
    }

    // Initialize the memory (optional)
    memset(shared_usernames, 0, MAX_USERS * 50 * sizeof(char));  // Set all memory to 0
}


int main() {
    initialize_shared_memory();
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Setup the server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // while (1) {
    //     printf("Waiting for connections...\n");

    //     // Accept client connections
    //     if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
    //         perror("Accept");
    //         exit(EXIT_FAILURE);
    //     }

    //     printf("Connection accepted\n");

    //     // Create a thread for each client
    //     pthread_t client_thread;
    //     int *new_sock = malloc(sizeof(int));
    //     *new_sock = new_socket;

    //     if (pthread_create(&client_thread, NULL, handle_client, (void *)new_sock) < 0) {
    //         perror("Could not create thread");
    //         return 1;
    //     }
        
    //     printf("Handler assigned\n");
    // }


    while (1) {
        printf("Waiting for connections...\n");

        // Accept client connections
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept");
            exit(EXIT_FAILURE);
        }

        printf("Connection accepted\n");

        // Create a child process for each client using fork()
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Child process
            close(server_fd); // Child doesn't need the server socket

            // Allocate memory for the socket and pass it to the handler
            int *new_sock = malloc(sizeof(int));
            *new_sock = new_socket;

            handle_client((void*)new_sock); // Pass the socket as a void pointer

            exit(0); // Terminate child process after handling the client
        } else {
            // Parent process
            close(new_socket); // Parent doesn't need the client socket
        }
    }

    return 0;
}
