#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "client.h"
#include "globals.h"
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include "input_processing.h"



char *client_fifo;
int server_fd;
int request_id = 0;

sem_t *get_semaphore() {
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        fprintf(stderr, "Error: Unable to create/open semaphore\n");
        exit(EXIT_FAILURE);
    }
    return sem;
}

void handle_sigpipe(int sig) {
    fprintf(stderr, "Error: Broken fifo, server might have closed the connection\n");
}

int initiliaze_client_fifo(){
    pid_t pid = getpid();
    client_fifo = (char *)malloc(20);
    sprintf(client_fifo, "/tmp/fifoclient.%d", pid);
    if(mkfifo(client_fifo, FILE_MODE) < 0){
        fprintf(stderr, "Error: Cannot create client fifo\n");
        return 1;
    }
    return 0;
}

int connect_to_server() {
    int attempts = 0;

    // Create the FIFO if it doesn't exist
    if (mkfifo(SERVER_ENDPOINT, 0666) < 0 && errno != EEXIST) {
        fprintf(stderr, "Error: Cannot create FIFO %s\n", SERVER_ENDPOINT);
        return -1;
    }

    // Try to open the FIFO in non-blocking mode
    while (attempts < MAX_ATTEMPTS) {
        server_fd = open(SERVER_ENDPOINT, O_WRONLY | O_NONBLOCK);
        
        if (server_fd >= 0) {
            fprintf(stdout, "Connected to server\n");
            return server_fd; // Successfully connected
        }

        if (errno == ENXIO) {
            // No readers present, wait and retry
            fprintf(stderr, "No readers present. Retrying in %d seconds...\n", RETRY_INTERVAL);
            sleep(RETRY_INTERVAL);
            attempts++;
        } else {
            // Other error occurred
            fprintf(stderr, "Error: Cannot connect to server\n");
            return -1;
        }
    }

    // Failed to connect after retries
    fprintf(stderr, "Error: Failed to connect to server after %d attempts\n", MAX_ATTEMPTS);
    return -1;
}

int send_request(void *request, size_t request_size) {
    sem_t *sem = get_semaphore();
    ssize_t bytes_written = 0;
    size_t total_written = 0;

    sem_wait(sem);

    while (total_written < request_size) {
        bytes_written = write(server_fd, request + total_written, request_size - total_written);
        
        if (bytes_written < 0) {
            if (errno == EINTR) {
                continue; // Retry if interrupted by a signal
            } else if (errno == EPIPE) {
                fprintf(stderr, "Error: Server closed the connection (EPIPE)\n");
                sem_post(sem);
                return 1;
            } else {
                fprintf(stderr, "Error: Cannot send request to server\n");
                sem_post(sem);
                return 1;
            }
        }

        total_written += bytes_written;
    }

    sem_post(sem);
    return 0;
}


int add_course(int roll_no, int course_code, int marks) {
    IPCMessage message;

    // Fill in the operation details
    strcpy(message.operation, "add_course");
    message.query_number = request_id++;
    strcpy(message.response_fifo, client_fifo);

    // Fill in the specific arguments for add_course
    message.args.add_course.arg1 = roll_no;
    message.args.add_course.arg2 = course_code;
    message.args.add_course.arg3 = marks;

    // Send the message
    if (send_request(&message, sizeof(IPCMessage)) == 1) {
        return 1;
    }

    return 0;
}


int delete_course(int roll_no, int course_code){
    IPCMessage message;

    strcpy(message.operation, "delete_course");
    message.query_number = request_id++;
    strcpy(message.response_fifo, client_fifo);

    message.args.delete_course.arg1 = roll_no;
    message.args.delete_course.arg2 = course_code;

    if(send_request(&message, sizeof(IPCMessage)) == 1){
        return 1;
    }

    return 0;
}

int edit_course(int roll_no, int course_code, int marks){
    IPCMessage message;

    strcpy(message.operation, "edit_course");
    message.query_number = request_id++;
    strcpy(message.response_fifo, client_fifo);

    message.args.edit_course.arg1 = roll_no;
    message.args.edit_course.arg2 = course_code;
    message.args.edit_course.arg3 = marks;

    if(send_request(&message, sizeof(IPCMessage)) == 1){
        return 1;
    }

    return 0;
}

int add_student(int roll_no, char *name, float cgpa){
    IPCMessage message;

    strcpy(message.operation, "add_student");
    message.query_number = request_id++;
    strcpy(message.response_fifo, client_fifo);

    message.args.add_student.arg1 = roll_no;
    strcpy(message.args.add_student.arg2, name);
    message.args.add_student.arg3 = cgpa;

    if(send_request(&message, sizeof(IPCMessage)) == 1){
        return 1;
    }

    return 0;
}

int delete_student(int roll_no){
    IPCMessage message;

    strcpy(message.operation, "delete_student");
    message.query_number = request_id++;
    strcpy(message.response_fifo, client_fifo);

    message.args.delete_student.arg1 = roll_no;

    if(send_request(&message, sizeof(IPCMessage)) == 1){
        return 1;
    }

    return 0;
}

int edit_student_cgpa(int roll_no, float cgpa){
    IPCMessage message;

    strcpy(message.operation, "edit_student");
    message.query_number = request_id++;
    strcpy(message.response_fifo, client_fifo);

    message.args.edit_student.arg1 = roll_no;
    message.args.edit_student.arg2 = cgpa;

    if(send_request(&message, sizeof(IPCMessage)) == 1){
        return 1;
    }

    return 0;
}

int write_database_into_output(){
    IPCMessage message;

    strcpy(message.operation, "write_database_into_output");
    message.query_number = request_id++;
    strcpy(message.response_fifo, client_fifo);

    if(send_request(&message, sizeof(IPCMessage)) == 1){
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]){
    signal(SIGPIPE, handle_sigpipe);
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE* file = input_processing(argv[1]);
    if(file == NULL){
        fprintf(stderr, "Error: Failed to open file.\n");
        return 1;
    }

    initiliaze_client_fifo();
    connect_to_server();

    if(parse(file) != 0){
        fprintf(stderr, "Error: Failed to parse file Completely.\n");
        fclose(file);
        return 1;
    }
    fclose(file);

    write_database_into_output();
    
    return 0;
}