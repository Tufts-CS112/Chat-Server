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
connection* create_connection(int client_socket_fd) {
    connection* connection = malloc(sizeof(connection));
    connection->client_socket_fd = client_socket_fd;
    connection->data_stored = 0;
    return connection;
}
//-------------------------------------------------------------------------------------------------
