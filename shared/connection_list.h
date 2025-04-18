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
connection* add_connection(connection_list** connection_list_head, int socket_fd); 
void add_connection_message(connection_list** connection_list_ref, int socket_fd, message* message, int bytes_received);
connection* get_connection(connection_list** connection_list_head, int socket_fd);
int get_socket(connection_list** connection_list_head, char* client_id);
message* get_CLIENT_LIST_message(char* client_id, connection_list** connection_list_head);
bool client_connected(char* client_id, connection_list** connection_list_head_ref);
bool connection_present(connection_list** connection_list_head, int socket_fd);
void print_connection_list(connection_list** connection_list_head);
bool duplicate_clients(connection_list** connection_list_head, char* client_ID);
bool remove_connection(connection_list** connection_list_ref, connection* connection_to_remove);
void free_connection_list(connection_list* connection_list_head);
connection* get_closest_connection_to_timeout(connection_list** connection_list_head_ref);
void remove_timeouts(connection_list** connection_list_head_ref, fd_set* master_FD_SET, int timeout);
void timeval_diff(struct timeval *start, struct timeval *end, struct timeval *diff);
bool timed_out(connection* connection, int timeout);

#endif
//-------------------------------------------------------------------------------------------------
