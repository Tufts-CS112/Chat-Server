//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      main.c 
// Usage:       ./a.out <port>
//*************************************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>
// #include <sys/types.h>  // For socket        
// #include <sys/socket.h> 
// #include <netinet/in.h> // Provides sockaddr_in struct
// #include <netdb.h>      // Provides hostent struct
// #include <stdbool.h>

#include "client.h"
#include "../shared/message.h"       
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
// ----GLOBAL VARIABLES----------------------------------------------------------------------------
#define HEADER_SIZE 50

//----FUNCTIONS------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    
    // Declare variables
    int SERVER_PORT; 

    // Get port number from argv
    if(argc != 3) {
        printf("Usage: %s <port> <client_ID> \n", argv[0]);
        return -1;
    }
    SERVER_PORT = atoi(argv[1]);

    // Initialize server
    int client_socket = connect_to_server(SERVER_PORT);

    // Send hello message (Client 1)
    message* hello_message = get_HELLO_message(argv[2]);
    printf("Writing message to server...\n");
    convert_message_to_network_byte_order(hello_message);
    write_to_server(client_socket, (char*) hello_message, HEADER_SIZE);
    free(hello_message);

    // Receive response from server
    printf("Receiving message from server...\n");
    message* message_response = malloc(sizeof(message));
    int bytes_received = read(client_socket, message_response, sizeof(message));
    printf("Received %d bytes from server\n", bytes_received);
    convert_message_to_host_byte_order(message_response);
    print_message(message_response);

    // printf("Receiving message from server...\n");
    message* message_response_two = malloc(sizeof(message));
    bytes_received = read(client_socket, message_response_two, sizeof(message));
    printf("Received %d bytes from server\n", bytes_received);
    convert_message_to_host_byte_order(message_response_two);
    print_message(message_response_two);

    while(1) {
        
    }
    // Free buffer
    free(message_response);
    // free(message_response_two);

    // Disconnect from server
    disconnect_from_server(client_socket);

    return 0;
}

//-------------------------------------------------------------------------------------------------