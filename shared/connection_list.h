//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      connection_list.h 
// Usage:       Header file for connection list: used by server for storing list of client 
//              connections
//*************************************************************************************************
#ifndef CONNECTION_LIST_H
#define CONNECTION_LIST_H

#include "connection.h"
#include <stdlib.h>

// ----GLOBAL VARIABLES----------------------------------------------------------------------------

// ----STRUCT--------------------------------------------------------------------------------------
// Message struct with padding removed
typedef struct connection_list{
    connection* connection;
    struct connection_list* next;
} connection_list;

//----FUNCTIONS------------------------------------------------------------------------------------
void add_connection(connection_list** connection_list_ref, struct sockaddr_in *client_addr);
void free_client_list(connection_list* connection_list_head);


#endif
//-------------------------------------------------------------------------------------------------
