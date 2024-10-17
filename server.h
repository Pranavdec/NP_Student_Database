#ifndef SERVER_H
#define SERVER_H

#include "globals.h"
#include "database.h"

// Function to set up the server socket and start listening.
int setup_server_socket();

// Function to handle client requests (runs in the same thread for simplicity).
void handle_client(int client_socket);

// Function to write the response back to the client over a socket.
void write_response_to_socket(int client_socket, char *response, int query_number);

// Handlers for various operations requested by the client.
void handle_add_student( int client_socket ,IPCMessage *request );
void handle_delete_student(int client_socket, IPCMessage *request);
void handle_edit_student(int client_socket, IPCMessage *request);

void handle_add_course(int client_socket ,IPCMessage *request);
void handle_delete_course(int client_socket ,IPCMessage *request);
void handle_edit_course(int client_socket ,IPCMessage *request);

// For debugging purposes (not used in production).
void handle_write_database_into_output(int client_socket ,IPCMessage *request);

#endif
