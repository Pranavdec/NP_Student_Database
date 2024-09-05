#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "globals.h"


void print_ipc_message(const IPCMessage *message) {
    printf("Operation: %s\n", message->operation);
    printf("Query Number: %d\n", message->query_number);
    printf("Response FIFO: %s\n", message->response_fifo);

    if (strcmp(message->operation, "add_student") == 0) {
        printf("Arguments: %d, %s, %f\n", 
                message->args.add_student.arg1, 
                message->args.add_student.arg2, 
                message->args.add_student.arg3);
    } else if (strcmp(message->operation, "edit_student") == 0) {
        printf("Arguments: %d, %f\n", 
                message->args.edit_student.arg1, 
                message->args.edit_student.arg2);
    } else if (strcmp(message->operation, "delete_student") == 0) {
        printf("Argument: %d\n", message->args.delete_student.arg1);
    } else if (strcmp(message->operation, "add_course") == 0) {
        printf("Arguments: %d, %d, %d\n", 
                message->args.add_course.arg1, 
                message->args.add_course.arg2, 
                message->args.add_course.arg3);
    } else if (strcmp(message->operation, "delete_course") == 0) {
        printf("Arguments: %d, %d\n", 
                message->args.delete_course.arg1, 
                message->args.delete_course.arg2);
    } else if (strcmp(message->operation, "edit_course") == 0) {
        printf("Arguments: %d, %d, %d\n", 
                message->args.edit_course.arg1, 
                message->args.edit_course.arg2, 
                message->args.edit_course.arg3);
    } else {
        printf("Unknown operation\n");
    }
}


int read_from_fifo(int fifo_fd) {
    IPCMessage message;
    ssize_t bytes_read;

    // Attempt to read the message structure from the FIFO
    bytes_read = read(fifo_fd, &message, sizeof(IPCMessage));
    
    if (bytes_read < 0) {
        fprintf(stderr, "Error: Cannot read from FIFO\n");
        return 1;
    } else if (bytes_read == 0) {
        fprintf(stderr, "Warning: No data in FIFO\n");
        return 1;
    } else if (bytes_read != sizeof(IPCMessage)) {
        fprintf(stderr, "Error: Incomplete read from FIFO, expected %ld bytes but got %ld bytes\n", 
                sizeof(IPCMessage), bytes_read);
        return 1;
    }

    // Successfully read the message, now print it to stdout
    print_ipc_message(&message);

    return 0;
}



int main() {
    int fifo_fd;

    // Open the FIFO for reading
    fifo_fd = open(SERVER_ENDPOINT, O_RDONLY);
    if (fifo_fd < 0) {
        fprintf(stderr, "Error: Cannot open FIFO for reading\n");
        return 1;
    }

    // Read from the FIFO and print the message
    while (1) {
        if (read_from_fifo(fifo_fd) != 0) {
            printf("Read error");
            break;
        }
    }

    // Close the FIFO
    close(fifo_fd);

    return 0;
}
