//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      main.c 
// Usage:       ./a.out <port>
//*************************************************************************************************
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>  // For socket        
// #include <sys/socket.h> 
// #include <netinet/in.h> // Provides sockaddr_in struct
// #include <netdb.h>      // Provides hostent struct
// #include <stdbool.h>

#include "server.h"
#include "message.h"       

// ----GLOBAL VARIABLES----------------------------------------------------------------------------

//----FUNCTIONS------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    
    // Declare variables
    int LISTENING_PORT; 

    // Get port number from argv
    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return -1;
    }
    LISTENING_PORT = atoi(argv[1]);

    // Initialize server
    initialize_server(LISTENING_PORT);
}

//-------------------------------------------------------------------------------------------------