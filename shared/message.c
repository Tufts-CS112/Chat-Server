//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      message.c
// Usage:       Implementation file for message
//*************************************************************************************************
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

// ----GLOBAL VARIABLES----------------------------------------------------------------------------


//----FUNCTIONS------------------------------------------------------------------------------------
// Print contents of message for debugging
void print_message(message* message) {
    printf("  ----------------------------------------\n");
    printf("  MESSAGE\n");
    printf("  Type: %d\n", message->type);
    printf("  Source: %s\n", message->source);
    printf("  Destination: %s\n", message->destination);
    printf("  Length: %d\n", message->length);
    printf("  Message ID: %d\n", message->message_ID);
    printf("  Data: %s\n", message->data);
    printf("----------------------------------------\n");
}

// Copy contents of message to buffer for writing to server
// char* message_to_buffer(message message){
//     char* buffer = malloc(sizeof(struct message));
//     memcpy(buffer, &message, sizeof(struct message));
//     return buffer;
// }

void convert_message_to_network_byte_order(message* message){
    message->type = htons(message->type);
    message->length = htonl(message->length);
    message->message_ID = htonl(message->message_ID);
}

void convert_message_to_host_byte_order(message* message){
    message->type = ntohs(message->type);
    message->length = ntohl(message->length);
    message->message_ID = ntohl(message->message_ID);
}


// Return HELLO message (as specified by specs)
message* get_HELLO_message(char* client_id) {
    if(strlen(client_id) > SOURCE_LENGTH) {
        printf("Client ID longer than maximum string length of %d characters\n", (int) SOURCE_LENGTH);
    } 
    message* message = malloc(sizeof(struct message));
    memset(message, 0, sizeof(struct message));
    message->type = 1;
    strncpy(message->source, client_id, sizeof(message->source)-1);
    message->source[sizeof(message->source)-1] = '\0';
    strncpy(message->destination, "Server", sizeof(message->destination)-1);
    message->destination[sizeof(message->destination)-1] = '\0';
    message->length = 0;
    message->message_ID = 0;
    return message;     
}

// Return HELLO_ACK message (as specified by specs)
message* get_HELLO_ACK_message(char* client_id) {
    message* message = malloc(sizeof(struct message));
    memset(message, 0, sizeof(struct message));
    message->type = 2;
    strncpy(message->source, "Server", sizeof(message->source)-1);
    strncpy(message->destination, client_id, sizeof(message->destination)-1);
    message->length = 0;
    message->message_ID = 0;
    return message;
}

// // Return LIST_REQUEST message (as specified by specs)
// message get_LIST_REQUEST_message(const char* client_id) {
    // message message;
    // memset(&message, 0, sizeof(message));
//     message.type = 3;
//     strcpy(message.source, client_id);
//     strcpy(message.destination, "Server");
//     message.length = 0;
//     message.message_ID = 0;
//     return message;
// }

// Return LIST_REQUEST message (as specified by specs)
// message get_CLIENT_LIST_message(const char* client_id) {
    // message message;
    // memset(&message, 0, sizeof(message));
//     message.type = 4;
//     strcpy(message.source, "Server");
//     strcpy(message.destination, client_id);
//     message.length = 0;
//     message.message_ID = 0;
//     return message;
// }


//-------------------------------------------------------------------------------------------------
