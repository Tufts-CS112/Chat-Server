//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      connection_list.c
// Usage:       Implementation file for connection list: used by server for storing list of client 
//              connections
//*************************************************************************************************
#include "connection_list.h"
#include <netinet/in.h> // Provides sockaddr_in struct

// ----GLOBAL VARIABLES----------------------------------------------------------------------------


//----FUNCTIONS------------------------------------------------------------------------------------

// Given ptr to head pointer, create new connection and add to connection list, modify
// header as needed
void add_connection(connection_list** connection_list_head, struct sockaddr_in *client_addr) {
    // Create connection
    connection* new_connection = get_connection(client_addr);
    // Create new node
    connection_list* new_connection_list = malloc(sizeof(connection_list));
    new_connection_list->connection = new_connection;
    new_connection_list->next = NULL; 

    // Add connection to end of list of connections    
    if(*connection_list_head == NULL) {
        *connection_list_head = new_connection_list;
    } else {
        connection_list* connection_list_ref = *connection_list_head;
        while(connection_list_ref->next != NULL) {
            connection_list_ref = connection_list_ref->next;
        }
        connection_list_ref->next = new_connection_list;
    }
}

// Given head of connection list, free list
void free_client_list(connection_list* connection_list_head) {
    if(connection_list_head != NULL) {
        connection_list* connection_list_current = connection_list_head;
        connection_list* connection_list_next = connection_list_current->next;

        free(connection_list_current->connection);
        free(connection_list_current);

        while(connection_list_next != NULL) {
            connection_list_current = connection_list_next;
            connection_list_next = connection_list_current->next;
            free(connection_list_current->connection);
            free(connection_list_current);  
        }
    }
}


//-------------------------------------------------------------------------------------------------
