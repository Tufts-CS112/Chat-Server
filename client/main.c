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

// ----GLOBAL VARIABLES----------------------------------------------------------------------------

//----FUNCTIONS------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    
    // Declare variables
    int SERVER_PORT; 

    // Get port number from argv
    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return -1;
    }
    SERVER_PORT = atoi(argv[1]);

    // Initialize server
    int client_socket = connect_to_server(SERVER_PORT);

    // Get hello message
    message hello_message = get_HELLO_message("Client1");
    // char* buffer_hello_message = message_to_buffer(hello_message);
    print_message(&hello_message);

    // Write message to server
    printf("Writing message to server...\n");
    write_to_server(client_socket, (char*) &hello_message);

    // Free buffer
    // free(buffer_hello_message);

    // Disconnect from server
    disconnect_from_server(client_socket);

    return 0;
}

//-------------------------------------------------------------------------------------------------