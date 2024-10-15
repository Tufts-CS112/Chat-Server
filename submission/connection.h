//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      connection.h 
// Usage:       Header file for connection struct: used by server in storing details of client 
//              connection
//*************************************************************************************************
#ifndef CONNECTION_H
#define CONNECTION_H

#include "message.h"

#include <stdlib.h>
#include <netinet/in.h> // Provides sockaddr_in struct

// ----GLOBAL VARIABLES----------------------------------------------------------------------------
#define MAX_IP_STRING_LENGTH 32
#define MAX_PORT_STRING_LENGTH 6
// ----STRUCT--------------------------------------------------------------------------------------
// Message struct with padding removed
typedef struct connection{
    int client_socket_fd;
    int data_stored;
    struct message* message;
} connection;

//----FUNCTIONS------------------------------------------------------------------------------------
connection* create_connection(int client_socket_fd);
#endif
//-------------------------------------------------------------------------------------------------
