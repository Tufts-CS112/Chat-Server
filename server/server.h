//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      server.h 
// Usage:       Header file for server
//*************************************************************************************************
#ifndef SERVER_H
#define SERVER_H

#include "../shared/connection_list.h"
// ----GLOBAL VARIABLES----------------------------------------------------------------------------

// ----STRUCT--------------------------------------------------------------------------------------

//----FUNCTIONS------------------------------------------------------------------------------------
int initialize_server(int PORT);
void send_message(int socket_fd, message* message);
void write_to_client(int client_socket, char* buffer);
void receive_data(int socket, connection_list** connection_list_head_ref);

#endif
//-------------------------------------------------------------------------------------------------
