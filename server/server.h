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
void send_message(int socket_fd, message* message, int message_size);
void write_to_client(int client_socket, char* buffer, int message_size);
// void receive_data(int socket, connection_list** connection_list_head_ref);
void receive_and_respond(int socket, connection_list** connection_list_head_ref, fd_set* master_FD_SET);
void server_response(int socket_fd, connection* connection, connection_list** connection_list_head_ref, fd_set* master_FD_SET);
struct timeval* get_timeout(connection_list** connection_list_head_ref);

#endif
//-------------------------------------------------------------------------------------------------
