//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      server.c
// Usage:       Implementation file for server
//*************************************************************************************************
#include "server.h"
#include "../shared/message.h"
#include "../shared/connection_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // For socket        
#include <sys/socket.h> 
#include <netinet/in.h> // Provides sockaddr_in struct
#include <arpa/inet.h>
#include <netdb.h>      // Provides hostent struct
#include <sys/select.h> // Provides select() functionality


// ----GLOBAL VARIABLES----------------------------------------------------------------------------
#define MAX_CLIENT_CONNECTIONS 10
#define HEADER_SIZE 50

//----FUNCTIONS------------------------------------------------------------------------------------
// Max function (since max isn't in stdlib)
int max(int a, int b) {
    return (a > b) ? a : b;
}

void send_message(int socket_fd, message* message) {
    // Send message
    write_to_client(socket_fd, (char*) message);
}

void write_to_client(int client_socket, char* buffer) {
    write(client_socket, buffer, sizeof(message));
}

// Receive data from client connection. If new client, add
// to client list. Parse data into messages and store appropriately
void receive_and_respond(int socket_fd, connection_list** connection_list_head_ref) {

    // Add connection if not already present
    if(!connection_present(connection_list_head_ref, socket_fd)) {
        // Add connection
        printf("Adding new connection to connection list\n");
        add_connection(connection_list_head_ref, socket_fd);
    }

    // Receive client data
    char* buffer = malloc(sizeof(message));
    int bytes_received = recv(socket_fd, buffer, sizeof(message), 0);
    printf("Bytes received: %d\n", bytes_received);
    connection* connection = get_connection(connection_list_head_ref, socket_fd);

    // Copy the connection bytes into messages, accounting for reception of partial
    // messages and multiple messages within a single transmission
    int remaining_connection_bytes = bytes_received;
    int connection_bytes_copied = 0;
    int bytes_to_copy;

    while(remaining_connection_bytes > 0) {
        // Copy bytes until header is filled, then convert to host-byte-order for readability
        if(connection->data_stored < HEADER_SIZE) {
            int remaining_header_bytes = HEADER_SIZE - connection->data_stored;
            bytes_to_copy = (remaining_header_bytes > remaining_connection_bytes) ? remaining_connection_bytes : remaining_header_bytes;
            printf("Received %d bytes, copying %d bytes\n", remaining_connection_bytes, bytes_to_copy);
            memcpy(connection->message + connection->data_stored, buffer+connection_bytes_copied, bytes_to_copy);
            connection->data_stored += bytes_to_copy;
            remaining_connection_bytes -= bytes_to_copy;
            connection_bytes_copied += bytes_to_copy;

            // Once complete header is received, convert to host byte order
            // to interpret message
            if(connection->data_stored == HEADER_SIZE) {
                printf("Complete header received. Converting to host-byte order\n");
                convert_message_to_host_byte_order(connection->message);
            }

            bool message_complete = (connection->data_stored >= HEADER_SIZE) && 
                                    (connection->data_stored == HEADER_SIZE + connection->message->length);
            if(message_complete) {
                printf("Full message received. Server responding...\n");
                server_response(socket_fd, connection, connection_list_head_ref);
            }
        } else {

            // Determine whether post-header bytes are meant for data of same message, 
            // or are beginning of new message
            int remaining_message_bytes = (HEADER_SIZE + connection->message->length) - connection->data_stored;
            printf("%d bytes remaining in transmission, %d bytes remaining to be filled in message\n", remaining_connection_bytes, remaining_message_bytes);
            if(remaining_message_bytes > 0) {
                // Copy bytes up until end of message, as prescribed by message->length field
                bytes_to_copy = (remaining_message_bytes > remaining_connection_bytes) ? remaining_connection_bytes : remaining_message_bytes;
                memcpy(connection->message + connection->data_stored, buffer+connection_bytes_copied, bytes_to_copy);
                connection->data_stored += bytes_to_copy;
                remaining_connection_bytes -= bytes_to_copy;
                connection_bytes_copied += bytes_to_copy;

                // If message is complete, respond accordingly
                if(HEADER_SIZE + connection->message->length == connection->data_stored) {
                    printf("Full message received. Server responding...\n");
                    server_response(socket_fd, connection, connection_list_head_ref);
                }
            } else {

                // Remaining connection bytes are a new message
                printf("Interpretting remaining bytes from transmission as new message\n");

                // Create new connection
                printf("Adding new connection to connection list\n");
                connection = add_connection(connection_list_head_ref, socket_fd);
            }
        }
    }
}

void server_response(int socket_fd, connection* connection, connection_list** connection_list_head_ref){
    struct message* message = connection->message;
    switch(message->type) {
        // Hello message
        case 1:
            // Check if already present
            if(!duplicate_clients(connection_list_head_ref, message->source)) {

                // Send back HELLO_ACK message
                printf("Sending HELLO_ACK\n");
                struct message* message_HELLO_ACK = get_HELLO_ACK_message(message->source);
                convert_message_to_network_byte_order(message_HELLO_ACK);
                send_message(socket_fd, message_HELLO_ACK);
                free(message_HELLO_ACK);

                // Send CLIENT_LIST message
                printf("Sending CLIENT_LIST\n");
                struct message* message_CLIENT_LIST = get_CLIENT_LIST_message(message->source, connection_list_head_ref);
                convert_message_to_network_byte_order(message_CLIENT_LIST);
                send_message(socket_fd, message_CLIENT_LIST);
                free(message_CLIENT_LIST);
            } else {

                // Return CLIENT_ALREADY_PRESENT error message
                printf("ClientID already present in connection list - sending error message and closing connection\n");
                struct message* error_CLIENT_ALREADY_PRESENT = get_CLIENT_ALREADY_PRESENT_error(message->source);
                convert_message_to_network_byte_order(error_CLIENT_ALREADY_PRESENT);
                send_message(socket_fd, error_CLIENT_ALREADY_PRESENT);
                printf("Printing connection list before and after removal of duplicate connection\n");
                // Remove from connection_list and close connection
                print_connection_list(connection_list_head_ref);
                remove_connection(connection_list_head_ref, connection);
                print_connection_list(connection_list_head_ref);
                close(socket_fd);
            }
            break;

        // List request
        case 3:
            struct message* message_CLIENT_LIST = get_CLIENT_LIST_message(message->source, connection_list_head_ref);
            convert_message_to_network_byte_order(message_CLIENT_LIST);
            send_message(socket_fd, message_CLIENT_LIST);
            free(message_CLIENT_LIST);
            printf("Printing connection list after sending client connection list\n");
            // Remove from connection_list and close connection
            remove_connection(connection_list_head_ref, connection);
            print_connection_list(connection_list_head_ref);
            break;
        // Chat message
        case 5:
            break;
        // Exit message
        case 6:
            break;
    }
}



int initialize_server(int PORT) {
    int client_socket, listening_socket;
    struct sockaddr_in client_addr, server_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    char* buffer = malloc(sizeof(message));

    // Create listening socket
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(listening_socket < 0) {
        perror("Error creating listening socket");
        return -1;
    }

    // Initialize fields of struct for server address
    memset(&server_addr, 0, sizeof(server_addr)); // Set structure to 0's, ensuring sin_zero is all zeros
    server_addr.sin_family = AF_INET;             // Set address family to IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;     // Set IP address to all IP addresses of machine
    server_addr.sin_port = htons(PORT);           // Set port number

    // Set socket options to allow reuse of the address (fixes "address already in use" bug)
    int opt = 1;
    if (setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Error setting socket options");
        close(listening_socket);
        return -1;
    }   

    // Bind socket to IP address and port
    if(bind(listening_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0 ){
        perror("Error binding socket");
        return -1;
    }

    // Listen for incoming connections requests on "listening socket"
    listen(listening_socket, MAX_CLIENT_CONNECTIONS);
    printf("Listening for incoming connection requests on port %d...\n\n", PORT);

    // Create file descriptor sets needed for select()
    fd_set master_set; 
    fd_set temp_set;

    // Clear file desriptor sets (as recommended by man. page)
    FD_ZERO(&master_set);
    FD_ZERO(&temp_set);

    // Add listening socket to master set and set to max file descriptor
    FD_SET(listening_socket, &master_set);
    int fdmax = listening_socket;

    // Create list of connections to maintain client connections
    struct connection_list* connection_list_head = NULL; 

    while(1){
        // Copy master set to temp set (to preserve master
        // set while select() modifies copy)
        temp_set = master_set;

        // Block until a socket is ready
        select(fdmax+1, &temp_set, NULL, NULL, NULL);

        // If master socket is ready, a new client has requested
        // connection. Accept new client and add to master set
        if(FD_ISSET(listening_socket, &temp_set)){
            client_socket = accept(listening_socket, (struct sockaddr *) &client_addr, &client_addr_size);
            printf("\nNew client connection. Server socket: %d, IP: %s, Port: %d\n", 
                    client_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            // Add new client to master set and update max file descriptor
            FD_SET(client_socket, &master_set);
            fdmax = max(client_socket, fdmax);
        } else {
            // If client is ready, receive data. Store and respond accordingly
            for(int socket = 0; socket <= fdmax; socket++) {
                if(FD_ISSET(socket, &temp_set)) {
                    // Store data appropriately and add client to
                    // client connection list if not already added
                    printf("Client is sending message:\n");
                    receive_and_respond(socket, &connection_list_head);

                    // Send response according to client request
                    // server_response(socket, &connection_list_head);
                    // close(socket);
                }
            }
        }
    }

    // Free data, close sockets
    // free(message);
    //close(socket)
    // free_client_list(connection_list_head);
    
    // Close sockets
    for(int socket = 0; socket <= fdmax; socket++) {
        close(socket);
    }
    return 0;
}


//-------------------------------------------------------------------------------------------------
