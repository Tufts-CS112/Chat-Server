//----HEADER---------------------------------------------------------------------------------------
// Author:      Sam Rolfe
// Date:        October 2024
// Script:      main.c 
// Usage:       ./a.out <port>
//*************************************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>
// #include <sys/types.h>  // For socket        
// #include <sys/socket.h> 
// #include <netinet/in.h> // Provides sockaddr_in struct
// #include <netdb.h>      // Provides hostent struct
// #include <stdbool.h>

#include "client.h"
#include "../shared/message.h"       
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
// ----GLOBAL VARIABLES----------------------------------------------------------------------------
#define HEADER_SIZE 50

//----FUNCTIONS------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    
    // Declare variables
    int SERVER_PORT; 

    // Get port number from argv
    if(argc != 3) {
        printf("Usage: %s <port> <client_ID> \n", argv[0]);
        return -1;
    }
    SERVER_PORT = atoi(argv[1]);

    // Initialize server
    int client_socket = connect_to_server(SERVER_PORT);

    int command;
    int bytes_received;
    message* message_response = malloc(sizeof(message));
    message* message_response_two = malloc(sizeof(message)); 
    while(1) {
        printf("\nEnter command: \n");
        scanf("%d", &command);

        switch(command){
            case 1:
                // Send hello message 
                message* hello_message = get_HELLO_message(argv[2]);
                printf("Writing message to server...\n");
                print_message(hello_message);
                convert_message_to_network_byte_order(hello_message);
                write_to_server(client_socket, (char*) hello_message, HEADER_SIZE);
                free(hello_message);

                // Receive response from server
                printf("Receiving message from server...\n");
                // message_response = malloc(sizeof(message));
                bytes_received = read(client_socket, message_response, sizeof(message));
                printf("Received %d bytes from server\n", bytes_received);
                convert_message_to_host_byte_order(message_response);
                print_message(message_response);

                // printf("Receiving message from server...\n");
                // message_response_two = malloc(sizeof(message));
                bytes_received = read(client_socket, message_response_two, sizeof(message));
                printf("Received %d bytes from server\n", bytes_received);
                convert_message_to_host_byte_order(message_response_two);
                print_message(message_response_two);
                
                break;

            case 3:
                printf("Sending client list request\n");
                message* list_request_message = get_LIST_REQUEST_message(argv[2]);  
                convert_message_to_network_byte_order(list_request_message);
                write_to_server(client_socket, (char*) list_request_message, HEADER_SIZE);
                free(list_request_message);  

                // Receive response from server
                printf("Receiving message from server...\n");
                // message_response = malloc(sizeof(message));
                bytes_received = read(client_socket, message_response, sizeof(message));
                printf("Received %d bytes from server\n", bytes_received);
                convert_message_to_host_byte_order(message_response);
                print_message(message_response);
                break;  

            case 5:
                char client_destination[20];
                char message_content[400];
                printf("Destination client_id: \n");
                scanf("%s", &client_destination);
                printf("Message: \n");
                scanf("%s", &message_content);
                message* CHAT_message = get_CHAT_message(argv[2], client_destination, message_content);
                print_message(CHAT_message);
                int bytes_to_write = HEADER_SIZE + CHAT_message->length;
                printf("Bytes to write to server: %d\n", bytes_to_write);
                convert_message_to_network_byte_order(CHAT_message);
                write_to_server(client_socket, (char*) CHAT_message, bytes_to_write);
                break;
            
            case 6:
                // Send EXIT message
                printf("Sending client EXIT message\n");
                message* EXIT_message = get_EXIT_message(argv[2]);  
                convert_message_to_network_byte_order(EXIT_message);
                write_to_server(client_socket, (char*) EXIT_message, HEADER_SIZE);
                free(EXIT_message);  
                break;

            case 8:
                // Send EXIT message
                printf("Waiting to receive messages\n");
                // message_response = malloc(sizeof(message));
                while(1){
                    bytes_received = read(client_socket, message_response, sizeof(message));
                    printf("Received %d bytes from server\n", bytes_received);
                    convert_message_to_host_byte_order(message_response);
                    print_message(message_response);
                }  
                break;
        }

    }

    // Free buffer
    free(message_response);
    free(message_response_two);

    // Disconnect from server
    disconnect_from_server(client_socket);

    return 0;
}

//-------------------------------------------------------------------------------------------------