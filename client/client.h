//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      client.h 
// Usage:       Header file for client
//*************************************************************************************************
#ifndef CLIENT_H
#define CLIENT_H

// ----GLOBAL VARIABLES----------------------------------------------------------------------------

// ----STRUCT--------------------------------------------------------------------------------------

//----FUNCTIONS------------------------------------------------------------------------------------
int connect_to_server(int LISTENING_PORT);
void disconnect_from_server(int client_socket);
void write_to_server(int client_socket, char* message, int bytes_to_read);

#endif
//-------------------------------------------------------------------------------------------------
