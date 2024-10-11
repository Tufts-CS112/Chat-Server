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

// ----GLOBAL VARIABLES----------------------------------------------------------------------------


//----FUNCTIONS------------------------------------------------------------------------------------
// Return HELLO message (as specified by specs)
message get_HELLO_message(char* client_id, size_t client_id_size) {
    message message;
    message.type = 1;
    strncpy(message.source, client_id, client_id_size);
    strcpy(message.destination, "Server");
    message.length = 0;
    message.message_ID = 0;
    return message;
}

// Return HELLO_ACK message (as specified by specs)
message get_HELLO_ACK_message(char* client_id, size_t client_id_size) {
    message message;
    message.type = 2;
    strcpy(message.source, "Server");
    strncpy(message.destination, client_id, client_id_size);
    message.length = 0;
    message.message_ID = 0;
    return message;
}

// Return LIST_REQUEST message (as specified by specs)
message get_LIST_REQUEST_message(char* client_id, size_t client_id_size) {
    message message;
    message.type = 3;
    strncpy(message.source, client_id, client_id_size);
    strcpy(message.destination, "Server");
    message.length = 0;
    message.message_ID = 0;
    return message;
}

// Return LIST_REQUEST message (as specified by specs)
// message get_CLIENT_LIST_message(char* client_id, size_t client_id_size) {
//     message message;
//     message.type = 4;
//     strcpy(message.source, "Server");
//     strncpy(message.destination, client_id, client_id_size);
//     message.length = 0;
//     message.message_ID = 0;
//     return message;
// }


//-------------------------------------------------------------------------------------------------
