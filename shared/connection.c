//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      connection.c
// Usage:       Implementation file for connection struct: used by server in storing details of 
//              client connection
//*************************************************************************************************
#include "connection.h"
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// ----GLOBAL VARIABLES----------------------------------------------------------------------------


//----FUNCTIONS------------------------------------------------------------------------------------
// Return connection object
connection* get_connection(struct sockaddr_in *client_addr) {
    connection* connection = malloc(sizeof(connection));
    char* client_IP = inet_ntoa(client_addr->sin_addr);
    int client_port = ntohs(client_addr->sin_port);
    snprintf(connection->ConnectionID, sizeof(connection->ConnectionID), "%s:%d", client_IP, client_port);
    return connection;
}
//-------------------------------------------------------------------------------------------------
