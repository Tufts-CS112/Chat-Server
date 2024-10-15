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
#include <string.h>

// ----GLOBAL VARIABLES----------------------------------------------------------------------------

#define HEADER_SIZE 50

//----FUNCTIONS------------------------------------------------------------------------------------

// Given ptr to head of connection_list, create new connection and 
// add to connection list, modify header as needed
connection* add_connection(connection_list** connection_list_head, int socket_fd) {
    
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
    return new_connection;
}

// Given pointer to head of connection list, socket_fd, and message, add message to connection 
// associated with socket_fd
void add_connection_message(connection_list** connection_list_ref, int socket_fd, struct message* message, int bytes_received) {
    connection* connection = get_connection(connection_list_ref, socket_fd);
    connection->data_stored = bytes_received;
    connection->message = message;
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

// Return true if two connections exist with the same client_ID.
bool duplicate_clients(connection_list** connection_list_head, char* client_ID) {
    int client_ID_count = 0;
    if(*connection_list_head != NULL) {
        connection_list* connection_list_ref = *connection_list_head;
        while(connection_list_ref != NULL) {
            if(connection_list_ref->connection->data_stored >= HEADER_SIZE) {
                if(strcmp(connection_list_ref->connection->message->source, client_ID) == 0){
                    client_ID_count += 1;
                    if(client_ID_count == 2) {
                        return true;
                    }
                }
            }
            connection_list_ref = connection_list_ref->next;
        }
    }
    return false;
}

// Given pointer to head of connection list, return list of 
// NULL terminated client IDs
message* get_CLIENT_LIST_message(char* client_id, connection_list** connection_list_head_ref){
    message* message_CLIENT_LIST = get_CLIENT_LIST_empty_message(client_id);
    // Get client ID's and add them to the Data field
    connection_list* connection_list_ref = *connection_list_head_ref;
    int offset = 0;
    while(connection_list_ref != NULL) {
        char* connection_client_id = connection_list_ref->connection->message->source;
        strncpy(message_CLIENT_LIST->data + offset, connection_client_id, strlen(connection_client_id));
        offset += strlen(connection_client_id);
        message_CLIENT_LIST->data[offset] = '\0';
        offset++;
        connection_list_ref = connection_list_ref->next;
    }
    message_CLIENT_LIST->length = offset;

    return message_CLIENT_LIST;
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
        printf("----------------------------------------\n");
        connection_list_ref = connection_list_ref->next; 
    }
}

// Given pointer to head of connection list and connection, remove connection
// from list
bool remove_connection(connection_list** connection_list_head_ref, connection* connection_to_remove){
    connection_list* connection_list_ref_prev = NULL;
    connection_list* connection_list_ref = *connection_list_head_ref;
    while(connection_list_ref != NULL) {
        if(connection_list_ref->connection == connection_to_remove) {
            printf("Found connection to remove\n");
            if(connection_list_ref_prev != NULL) {
                connection_list_ref_prev->next = connection_list_ref->next;
            } else {
                *connection_list_head_ref = connection_list_ref->next;
            }
            free(connection_list_ref->connection->message);
            free(connection_list_ref->connection);
            free(connection_list_ref);
            return true;
        } else{
            connection_list_ref_prev = connection_list_ref;
            connection_list_ref = connection_list_ref->next;
        }
    }
    return false;
}




//-------------------------------------------------------------------------------------------------
