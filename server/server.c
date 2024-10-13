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
// #define MAX_DATA_SIZE 400

//----FUNCTIONS------------------------------------------------------------------------------------
// Max function (since max isn't in stdlib)
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Receive data from client connection. If new client, add
// to client list. Parse data into messages and store appropriately
void receive_data(int socket, connection_list** connection_list_head_ref) {

    // If not already present, add connection to connection list
    if(!connection_present(connection_list_head_ref, socket)) {
        printf("Adding new connection to connection list\n");
        add_connection(connection_list_head_ref, socket);
    }

    // Receive client data
    char* buffer = malloc(sizeof(message));
    int bytes_received = recv(socket, buffer, sizeof(message), 0);
    printf("Bytes received: %d\n", bytes_received);

    // Store client data
    connection* connection = get_connection(connection_list_head_ref, socket);
    if(connection->data_stored == 0) {
        message* message = malloc(sizeof(message));
        memcpy(message, buffer, bytes_received);
        add_connection_message(connection_list_head_ref, socket, message, bytes_received);
    }
    print_connection_list(connection_list_head_ref);
    free(buffer);
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

            // Add new client to master set and update max file descriptor
            FD_SET(client_socket, &master_set);
            fdmax = max(client_socket, fdmax);
        } else {
            // If client is ready, receive data. Store and respond accordingly
            for(int socket = 0; socket <= fdmax; socket++) {
                if(FD_ISSET(socket, &temp_set)) {
                    printf("Client is sending message:\n");
                    // Check whether client has already been added. If not, add
                    receive_data(socket, &connection_list_head);
                    close(socket);
                }
            }
        }
    }

    // Free data, close sockets
    // free(message);
    //close(socket)
    // free_client_list(connection_list_head);
    
    // Close sockets
    for(int socket = 0; socket <= fdmax; socket++) {
        close(socket);
    }
    return 0;
}


//-------------------------------------------------------------------------------------------------
