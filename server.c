#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "server.h"

int server_fd;

int setup_server_fifo() {
    // Create the server FIFO if it doesn't exist
    if (mkfifo(SERVER_ENDPOINT, 0666) < 0 && errno != EEXIST) {
        fprintf(stderr, "Error: Cannot create FIFO %s\n", SERVER_ENDPOINT);
        return -1;
    }

    // Open the server FIFO for reading
    server_fd = open(SERVER_ENDPOINT, O_RDONLY);
    if (server_fd < 0) {
        fprintf(stderr, "Error: Cannot open FIFO %s\n", SERVER_ENDPOINT);
        return -1;
    }

    return 0;
}

void handle_request(IPCMessage *request) {
    // Determine the operation and call the appropriate function
    if (strcmp(request->operation, "add_student") == 0) {
        handle_add_student(request);
    } else if (strcmp(request->operation, "delete_student") == 0) {
        handle_delete_student(request);
    } else if (strcmp(request->operation, "edit_student") == 0) {
        handle_edit_student(request);
    } else if (strcmp(request->operation, "add_course") == 0) {
        handle_add_course(request);
    } else if (strcmp(request->operation, "delete_course") == 0) {
        handle_delete_course(request);
    } else if (strcmp(request->operation, "edit_course") == 0) {
        handle_edit_course(request);
    } else if (strcmp(request->operation, "write_database") == 0) {
        handle_write_database_into_output(request);
    } else {
        fprintf(stderr, "Error: Unknown operation %s\n", request->operation);
    }

    return;
}

void write_response_to_fifo(char *response, int query_number, char *fifo_path) {

    CLIENTRESPONSE client_response;
    client_response.query_number = query_number;
    strcpy(client_response.response, response);

    // printf("Writing response");
    int response_fd = open(fifo_path, O_WRONLY);
    if (response_fd < 0) {
        fprintf(stderr, "Error: Cannot open response FIFO %s\n", fifo_path);
        return;
    }

    ssize_t bytes_written = write(response_fd, &client_response, sizeof(CLIENTRESPONSE));
    if (bytes_written < 0) {
        fprintf(stderr, "Error: Cannot write to response FIFO %s\n", fifo_path);
    }else{
        fprintf(stdout, "%ld bytes written\n", bytes_written);
    }

    close(response_fd);
}


void handle_add_student(IPCMessage *request) {
    char response[256];  // Assuming the response fits within 256 characters
    int result = add_student(request->args.add_student.arg1, request->args.add_student.arg2, request->args.add_student.arg3, response);
    
    // Debug statements
    printf("In handle_add_student\n");
    printf("Response from add_student: %s\n", response);

    // Check the result and print a success/failure message
    if (result == 0) {
        printf("Student added successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to add student\n");
    }

    // Debug statement for FIFO path
    // printf("Writing response to FIFO: %s\n", request->response_fifo);

    //The above printf is printing, the below printf is not printing

    // Write the response to the FIFO
    // printf("Writing response");
    // printf("\n");

    write_response_to_fifo(response, request->query_number,request->response_fifo);
}


void handle_delete_student(IPCMessage *request) {
    char response[256];
    int result = delete_student(request->args.delete_student.arg1, response);
    
    if (result == 0) {
        printf("Student deleted successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to delete student\n");
    }
    
    write_response_to_fifo(response, request->query_number,request->response_fifo);
}

void handle_edit_student(IPCMessage *request) {
    char response[256];
    int result = edit_student_cgpa(request->args.edit_student.arg1, request->args.edit_student.arg2, response);
    
    if (result == 0) {
        printf("Student edited successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to edit student\n");
    }
    
    write_response_to_fifo(response, request->query_number,request->response_fifo);
}

void handle_add_course(IPCMessage *request) {
    char response[256];
    int result = add_course(request->args.add_course.arg1, request->args.add_course.arg2, request->args.add_course.arg3, response);
    
    if (result == 0) {
        printf("Course added successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to add course\n");
    }
    
    write_response_to_fifo(response, request->query_number,request->response_fifo);
}

void handle_delete_course(IPCMessage *request) {
    char response[256];
    int result = delete_course(request->args.delete_course.arg1, request->args.delete_course.arg2, response);
    
    if (result == 0) {
        printf("Course deleted successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to delete course\n");
    }
    
    write_response_to_fifo(response, request->query_number,request->response_fifo);
}

void handle_edit_course(IPCMessage *request) {
    char response[256];
    int result = edit_course(request->args.edit_course.arg1, request->args.edit_course.arg2, request->args.edit_course.arg3, response);
    
    if (result == 0) {
        printf("Course edited successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to edit course\n");
    }
    
    write_response_to_fifo(response, request->query_number,request->response_fifo);
}

void handle_write_database_into_output(IPCMessage *request) {
    char response[256];
    int result = write_database_into_output(response);
    
    if (result == 0) {
        printf("Database written to output successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to write database to output\n");
    }
    
    write_response_to_fifo(response, request->query_number,request->response_fifo);
}


int main() {
    IPCMessage request;

    // // Create the FIFO (if it doesn't already exist)
    // if (mkfifo(SERVER_ENDPOINT, 0666) < 0 && errno != EEXIST) {
    //     fprintf(stderr, "Error: Cannot create FIFO %s\n", SERVER_ENDPOINT);
    //     exit(EXIT_FAILURE);
    // }

    // // Open the FIFO in read-only mode
    // server_fd = open(SERVER_ENDPOINT, O_RDONLY);
    // if (server_fd < 0) {
    //     fprintf(stderr, "Error: Cannot open FIFO %s\n", SERVER_ENDPOINT);
    //     exit(EXIT_FAILURE);
    // }

    setup_server_fifo();

    printf("Server is running and waiting for client requests...\n");

    // Infinite loop to keep the server running
    while (1) {
        // Read the incoming request from the FIFO
        ssize_t bytes_read = read(server_fd, &request, sizeof(IPCMessage));
        if (bytes_read > 0) {
            // Process the request
            handle_request(&request);
        } else if (bytes_read == 0) {
            // No data read, continue waiting for client requests
            continue;
        } else {
            // Error occurred while reading
            fprintf(stderr, "Error: Cannot read from FIFO %s\n", SERVER_ENDPOINT);
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        
    }

    // Close the server FIFO (this code will never be reached in an infinite loop)
    close(server_fd);
    return 0;
}
