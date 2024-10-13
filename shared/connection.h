//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      connection.h 
// Usage:       Header file for connection struct: used by server in storing details of client 
//              connection
//*************************************************************************************************
#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdlib.h>
#include <netinet/in.h> // Provides sockaddr_in struct

// ----GLOBAL VARIABLES----------------------------------------------------------------------------
#define MAX_IP_STRING_LENGTH 32
#define MAX_PORT_STRING_LENGTH 6
// ----STRUCT--------------------------------------------------------------------------------------
// Message struct with padding removed
typedef struct connection{
    char ConnectionID[MAX_IP_STRING_LENGTH + MAX_PORT_STRING_LENGTH];
} connection;

//----FUNCTIONS------------------------------------------------------------------------------------
connection* get_connection(struct sockaddr_in *client_addr);
#endif
//-------------------------------------------------------------------------------------------------
