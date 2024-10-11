//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      client.c
// Usage:       Implementation file for client
//*************************************************************************************************
#include "client.h"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h> // Provides sockaddr_in struct
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
// #include <sys/select.h> // Provides select() functionality
// #include <netinet/in.h> // Provides sockaddr_in struct


// ----GLOBAL VARIABLES----------------------------------------------------------------------------


//----FUNCTIONS------------------------------------------------------------------------------------

void connect_to_server(int SERVER_PORT){
    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket for server connection
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Initialize fields of struct for server address
    memset(&server_addr, 0, sizeof(server_addr));  // Set structure to 0's, ensuring sin_zero is all zeros
    server_addr.sin_family = AF_INET;              // Set address family to IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;      // Set IP address to all IP addresses of machine
    server_addr.sin_port = htons(SERVER_PORT);     // Set port number

    // Connect to server
    if(connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Unable to connect to server");
        close(client_socket);
    }

    printf("Connected to the server\n");

    close(client_socket);

}



//-------------------------------------------------------------------------------------------------
