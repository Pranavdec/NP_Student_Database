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
    } else if (strcmp(request->operation, "write_database_into_output") == 0) {
        handle_write_database_into_output(request);
    } else {
        fprintf(stderr, "Error: Unknown operation %s\n", request->operation);
    }
}

void handle_add_student(IPCMessage *request) {
    if (add_student(request->args.add_student.arg1, request->args.add_student.arg2, request->args.add_student.arg3) == 0) {
        printf("Student added successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to add student\n");
    }
}

void handle_delete_student(IPCMessage *request) {
    if (delete_student(request->args.delete_student.arg1) == 0) {
        printf("Student deleted successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to delete student\n");
    }
}

void handle_edit_student(IPCMessage *request) {
    if (edit_student_cgpa(request->args.edit_student.arg1, request->args.edit_student.arg2) == 0) {
        printf("Student edited successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to edit student\n");
    }
}

void handle_add_course(IPCMessage *request) {
    if (add_course(request->args.add_course.arg1, request->args.add_course.arg2, request->args.add_course.arg3) == 0) {
        printf("Course added successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to add course\n");
    }
}

void handle_delete_course(IPCMessage *request) {
    if (delete_course(request->args.delete_course.arg1, request->args.delete_course.arg2) == 0) {
        printf("Course deleted successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to delete course\n");
    }
}

void handle_edit_course(IPCMessage *request) {
    if (edit_course(request->args.edit_course.arg1, request->args.edit_course.arg2, request->args.edit_course.arg3) == 0) {
        printf("Course edited successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to edit course\n");
    }
}

void handle_write_database_into_output(IPCMessage *request) {
    if (write_database_into_output() == 0) {
        printf("Database written to output successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to write database to output\n");
    }
}

int main() {
    int server_fd;
    IPCMessage request;

    // Create the FIFO (if it doesn't already exist)
    if (mkfifo(SERVER_ENDPOINT, 0666) < 0 && errno != EEXIST) {
        fprintf(stderr, "Error: Cannot create FIFO %s\n", SERVER_ENDPOINT);
        exit(EXIT_FAILURE);
    }

    // Open the FIFO in read-only mode
    server_fd = open(SERVER_ENDPOINT, O_RDONLY);
    if (server_fd < 0) {
        fprintf(stderr, "Error: Cannot open FIFO %s\n", SERVER_ENDPOINT);
        exit(EXIT_FAILURE);
    }

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
