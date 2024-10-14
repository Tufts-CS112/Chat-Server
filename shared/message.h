//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      message.h 
// Usage:       Header file for message
//*************************************************************************************************
#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdlib.h>

// ----GLOBAL VARIABLES----------------------------------------------------------------------------
#define SOURCE_LENGTH 20
#define DESTINATION_LENGTH 20
#define MAX_DATA_LENGTH 400

// ----STRUCT--------------------------------------------------------------------------------------
// Message struct with padding removed
typedef struct __attribute__((__packed__)) message {
    short int type; 
    char source[SOURCE_LENGTH];
    char destination[DESTINATION_LENGTH];
    int length;
    int message_ID; 
    char data[MAX_DATA_LENGTH];
} message;

//----FUNCTIONS------------------------------------------------------------------------------------

void print_message(message* message);
message* get_HELLO_message(char* client_id);
message* get_HELLO_ACK_message(char* client_id);
message* get_CLIENT_LIST_empty_message(char* client_id);
message* get_CLIENT_ALREADY_PRESENT_error(char* client_id);
void convert_message_to_network_byte_order(message* message);
void convert_message_to_host_byte_order(message* message);
#endif
//-------------------------------------------------------------------------------------------------
