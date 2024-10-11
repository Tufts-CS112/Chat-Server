//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      server.c
// Usage:       Implementation file for server
//*************************************************************************************************
#include "server.h"
#include <string.h>
// #include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h> // Provides select() functionality
#include <netinet/in.h> // Provides sockaddr_in struct


// ----GLOBAL VARIABLES----------------------------------------------------------------------------
#define BUFFER_MAX_SIZE 512  // Confirm size

//----FUNCTIONS------------------------------------------------------------------------------------
// Max function (since max isn't in stdlib)
int max(int a, int b) {
    return (a > b) ? a : b;
}

void initialize_server(int LISTENING_SOCKET) {
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    char* buffer = malloc((size_t) BUFFER_MAX_SIZE);

    // Create file descriptor sets needed for select()
    fd_set master_set; 
    fd_set temp_set;

    // Clear file desriptor sets (as recommended by man. page)
    FD_ZERO(&master_set);
    FD_ZERO(&temp_set);

    // Add listening socket to master set and set to max file descriptor
    FD_SET(LISTENING_SOCKET, &master_set);
    int fdmax = LISTENING_SOCKET;

    // while(1){
    //     // Copy master set to temp set (to preserve master
    //     // set while select() modifies copy)
    //     temp_set = master_set;

    //     // Block until a socket is ready
    //     select(fdmax+1, &temp_set, NULL, NULL, NULL);

    //     // If master socket is ready, a new client has requested
    //     // connection. Accept new client and add to master set
    //     if(FD_ISSET(LISTENING_SOCKET, &temp_set)){
    //         printf("A new client wants to connect\n");
    //         client_socket = accept(LISTENING_SOCKET, (struct sockaddr *) &client_addr, &client_addr_size);

    //         // Add new client to master set and update max file descriptor
    //         FD_SET(client_socket, &master_set);
    //         fdmax = max(client_socket, fdmax);
    //     } else {
    //         for(int socket = 0; socket <= fdmax; socket++) {
    //             if(FD_ISSET(socket, &temp_set)) {
    //                 int bytes_received = recv(socket, buffer, (size_t) BUFFER_MAX_SIZE-1, 0);
    //             }
    //         }
    //     }
    // }
    free(buffer);
}


//-------------------------------------------------------------------------------------------------
