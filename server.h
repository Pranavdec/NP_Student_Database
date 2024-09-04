#ifndef SERVER_H
#define SERVER_H

#include "globals.h"
#include "database.h"

int setup_server_fifo();
void handle_request(IPCMessage *request);
void write_response_to_fifo(char *response, int query_number, char *fifo_path);

void handle_add_student(IPCMessage *request);
void handle_delete_student(IPCMessage *request);
void handle_edit_student(IPCMessage *request);

void handle_add_course(IPCMessage *request);
void handle_delete_course(IPCMessage *request);
void handle_edit_course(IPCMessage *request);

void handle_write_database_into_output(IPCMessage *request);

#endif
