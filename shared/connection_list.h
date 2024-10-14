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
#include "message.h"
#include <stdlib.h>
#include <stdbool.h>

// ----GLOBAL VARIABLES----------------------------------------------------------------------------

// ----STRUCT--------------------------------------------------------------------------------------
// Message struct with padding removed
typedef struct connection_list{
    connection* connection;
    struct connection_list* next;
} connection_list;

//----FUNCTIONS------------------------------------------------------------------------------------
void add_connection(connection_list** connection_list_ref, int socket_fd);
void add_connection_message(connection_list** connection_list_ref, int socket_fd, message* message, int bytes_received);
connection* get_connection(connection_list** connection_list_head, int socket_fd);
message* get_CLIENT_LIST_message(char* client_id, connection_list** connection_list_head);
bool connection_present(connection_list** connection_list_head, int socket_fd);
void free_client_list(connection_list* connection_list_head);
void print_connection_list(connection_list** connection_list_head);
bool duplicate_clients(connection_list** connection_list_head, char* client_ID);
bool remove_connection(connection_list** connection_list_ref, connection* connection_to_remove);


#endif
//-------------------------------------------------------------------------------------------------
