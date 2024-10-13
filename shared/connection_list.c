//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      connection_list.c
// Usage:       Implementation file for connection list: used by server for storing list of client 
//              connections
//*************************************************************************************************
#include "connection_list.h"
#include <netinet/in.h> // Provides sockaddr_in struct
#include <stdbool.h>
#include <stdio.h>

// ----GLOBAL VARIABLES----------------------------------------------------------------------------


//----FUNCTIONS------------------------------------------------------------------------------------

// Given ptr to head pointer, create new connection and add to connection list, modify
// header as needed
void add_connection(connection_list** connection_list_head, int socket_fd) {
    // Create connection
    connection* new_connection = create_connection(socket_fd);
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

// Given pointer to head of connection list and socket_fd, return pointer to connection
// associated with socket_fd. If none exists, return NULL
connection* get_connection(connection_list** connection_list_head, int socket_fd) {
    connection* connection = NULL;
    connection_list* connection_list_ref = *connection_list_head;
    while(connection_list_ref != NULL) {
        if(connection_list_ref->connection->client_socket_fd == socket_fd) {
            connection = connection_list_ref->connection;
            break;
        }
        connection_list_ref = connection_list_ref->next;
    }
    return connection;
}

// Given pointer to head of connection list and socket_fd, return true
// if connection is already present in list, else false
bool connection_present(connection_list** connection_list_head, int socket_fd) {
    connection* connection = get_connection(connection_list_head, socket_fd);
    return connection != NULL;
}

// Given pointer to head of connection list, socket_fd, and message, add message to connection 
// associated with socket_fd
void add_connection_message(connection_list** connection_list_ref, int socket_fd, struct message* message, int bytes_received) {
    connection* connection = get_connection(connection_list_ref, socket_fd);
    connection->data_stored = bytes_received;
    connection->message = message;
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


void print_connection_list(connection_list** connection_list_head_ref) {
    connection_list* connection_list_ref = *connection_list_head_ref;
    while(connection_list_ref != NULL){
        printf("----------------------------------------\n");
        printf("CONNECTION\n");
        printf("client_socket_fd: %d\n", connection_list_ref->connection->client_socket_fd);
        printf("data stored: %d\n", connection_list_ref->connection->data_stored);
        printf("Message: \n");
        print_message(connection_list_ref->connection->message);
        printf("----------------------------------------\n\n");
        connection_list_ref = connection_list_ref->next; 
    }
}




//-------------------------------------------------------------------------------------------------
