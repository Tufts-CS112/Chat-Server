//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      server.c
// Usage:       Implementation file for server
//*************************************************************************************************
#include "server.h"
#include "../shared/message.h"
#include "../shared/connection_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // For socket        
#include <sys/socket.h> 
#include <netinet/in.h> // Provides sockaddr_in struct
#include <arpa/inet.h>
#include <netdb.h>      // Provides hostent struct
#include <sys/select.h> // Provides select() functionality


// ----GLOBAL VARIABLES----------------------------------------------------------------------------
#define MAX_CLIENT_CONNECTIONS 10

//----FUNCTIONS------------------------------------------------------------------------------------
// Max function (since max isn't in stdlib)
int max(int a, int b) {
    return (a > b) ? a : b;
}

int initialize_server(int PORT) {
    int client_socket, listening_socket;
    struct sockaddr_in client_addr, server_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    char* buffer = malloc(sizeof(message));

    // Create listening socket
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(listening_socket < 0) {
        perror("Error creating listening socket");
        return -1;
    }

    // Initialize fields of struct for server address
    memset(&server_addr, 0, sizeof(server_addr)); // Set structure to 0's, ensuring sin_zero is all zeros
    server_addr.sin_family = AF_INET;             // Set address family to IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;     // Set IP address to all IP addresses of machine
    server_addr.sin_port = htons(PORT);           // Set port number

    // Set socket options to allow reuse of the address (fixes "address already in use" bug)
    int opt = 1;
    if (setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Error setting socket options");
        close(listening_socket);
        return -1;
    }   

    // Bind socket to IP address and port
    if(bind(listening_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0 ){
        perror("Error binding socket");
        return -1;
    }

    // Listen for incoming connections requests on "listening socket"
    listen(listening_socket, MAX_CLIENT_CONNECTIONS);
    printf("Listening for incoming connection requests on port %d...\n\n", PORT);

    // Create file descriptor sets needed for select()
    fd_set master_set; 
    fd_set temp_set;

    // Clear file desriptor sets (as recommended by man. page)
    FD_ZERO(&master_set);
    FD_ZERO(&temp_set);

    // Add listening socket to master set and set to max file descriptor
    FD_SET(listening_socket, &master_set);
    int fdmax = listening_socket;

    // Create list of connections to maintain client connections
    struct connection_list* connection_list_head = NULL; 

    while(1){
        // Copy master set to temp set (to preserve master
        // set while select() modifies copy)
        temp_set = master_set;

        // Block until a socket is ready
        select(fdmax+1, &temp_set, NULL, NULL, NULL);

        // If master socket is ready, a new client has requested
        // connection. Accept new client and add to master set
        if(FD_ISSET(listening_socket, &temp_set)){
            client_socket = accept(listening_socket, (struct sockaddr *) &client_addr, &client_addr_size);
            printf("New client connection. Server socket: %d, IP: %s, Port: %d\n", 
                    client_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            add_connection(&connection_list_head, &client_addr);

            // Add new client to master set and update max file descriptor
            FD_SET(client_socket, &master_set);
            fdmax = max(client_socket, fdmax);
        } else {
            for(int socket = 0; socket <= fdmax; socket++) {
                // if(FD_ISSET(socket, &temp_set)) {
                //     printf("Client is sending message\n");
                //     int bytes_received = recv(socket, buffer, sizeof(message)-1, 0);
                // }
            }
        }
    }

    // Free data 
    free(buffer);
    free_client_list(connection_list_head);
    
    // Close sockets
    for(int socket = 0; socket <= fdmax; socket++) {
        close(socket);
    }
    return 0;
}


//-------------------------------------------------------------------------------------------------
