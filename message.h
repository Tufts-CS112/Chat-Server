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
    
// ----STRUCT--------------------------------------------------------------------------------------
// Message struct with padding removed
typedef struct __attribute__((__packed__)) message {
    short int type; 
    char source[20];
    char destination[20];
    int length;
    int message_ID; 
    char data[400];
} message;

//----FUNCTIONS------------------------------------------------------------------------------------

message get_HELLO_message(char* client_id, size_t client_id_size);
message get_HELLO_ACK_message(char* client_id, size_t client_id_size);
message get_LIST_REQUEST_message(char* client_id, size_t client_id_size);

#endif
//-------------------------------------------------------------------------------------------------
