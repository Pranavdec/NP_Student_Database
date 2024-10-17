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
#include <sys/socket.h>
#include <arpa/inet.h>
#include "input_processing.h"




char *client_fifo;
int socket_fd;
int request_id = 0;
int parse_complete = 0;

sem_t *get_semaphore() {
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        fprintf(stderr, "Error: Unable to create/open semaphore\n");
        exit(EXIT_FAILURE);
    }
    return sem;
}

void handle_sigpipe() {
    fprintf(stderr, "Error: Broken fifo, server might have closed the connection\n");
}

int initialize_client_socket(){
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0){
        fprintf(stderr, "Socket creation Failed");
        exit(EXIT_FAILURE);
    }
    return 0;   
}

int connect_to_server(char* ip, char* port_c) {
    struct  sockaddr_in servaddr;
    char *endptr;
    long port = strtol(port_c, &endptr, 10);
    if (*endptr != '\0' || port < 1 || port > 65535) {
        fprintf(stderr, "Invalid port number");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <=0 ){
        fprintf(stderr,"Invalid IPAddress or Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        if (errno == ETIMEDOUT)
            fprintf(stderr, "Connection timed out\n");
        else if (errno == ECONNREFUSED)
            fprintf(stderr, "Connection refused\n");
        else if (errno == EHOSTUNREACH || errno == ENETUNREACH)
            fprintf(stderr, "Host or network unreachable\n");
        else if (errno == ECONNABORTED)
            fprintf(stderr, "Connection aborted\n");
        else
            fprintf(stderr, "Connection failed\n");

        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    return 0;
}

int send_request(void *request, size_t request_size) {
    sem_t *sem = get_semaphore();

    sem_wait(sem);

    if(writen(socket_fd, request,request_size) <=0){
        fprintf(stderr,"Error to send data on tcp.");
        exit(EXIT_FAILURE);
    }

    sem_post(sem);
    return 0;
}

int add_course(int roll_no, int course_code, int marks) {
    IPCMessage message;

    strcpy(message.operation, "add_course");
    message.query_number = request_id++;
    // strcpy(message.response_fifo, client_fifo);

    message.args.add_course.arg1 = roll_no;
    message.args.add_course.arg2 = course_code;
    message.args.add_course.arg3 = marks;

    if (send_request(&message, sizeof(IPCMessage)) == 1) {
        return 1;
    }

    return 0;
}

int delete_course(int roll_no, int course_code){
    IPCMessage message;

    strcpy(message.operation, "delete_course");
    message.query_number = request_id++;
    // strcpy(message.response_fifo, client_fifo);

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
    // strcpy(message.response_fifo, client_fifo);

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
    // strcpy(message.response_fifo, client_fifo);

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
    // strcpy(message.response_fifo, client_fifo);

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
    // strcpy(message.response_fifo, client_fifo);

    message.args.edit_student.arg1 = roll_no;
    message.args.edit_student.arg2 = cgpa;

    if(send_request(&message, sizeof(IPCMessage)) == 1){
        return 1;
    }

    return 0;
}

int write_database_into_output(){
    IPCMessage message;

    strcpy(message.operation, "write_database");
    message.query_number = request_id++;
    // strcpy(message.response_fifo, client_fifo);

    if(send_request(&message, sizeof(IPCMessage)) == 1){
        return 1;
    }

    return 0;
}

void *read_response() {
    CLIENTRESPONSE client_response;

    while (1) {
        if(readn(socket_fd, &client_response,sizeof(CLIENTRESPONSE)) <=0){
            fprintf(stderr,"Error to send data on tcp.");
            exit(EXIT_FAILURE);

        } else {
            fprintf(stdout, "Query number: %d\n", client_response.query_number);
            fprintf(stdout, "Received response: %s\n", client_response.response);
            

            if (client_response.query_number == request_id -1 && parse_complete == 1) {
                fprintf(stdout, "Response for request_id %d received.\n", request_id);
                break; 
            }
        }
    }

    return NULL;
}

pthread_t create_response_reading_thread(){
    pthread_t thread;
    pthread_create(&thread, NULL, read_response, NULL);
    return thread;
}


int main(int argc, char* argv[]){
    signal(SIGPIPE, handle_sigpipe);
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input_file> <Server IP> <Port>\n", argv[0]);
        return 1;
    }

    FILE* file = input_processing(argv[1]);

    initialize_client_socket();
    connect_to_server(argv[2], argv[3]);
    pthread_t thread =create_response_reading_thread();

    if(parse(file) != 0){
        fprintf(stderr, "Error: Failed to parse file Completely.\n");
        fclose(file);
        return 1;
    }
    else{
        parse_complete = 1;
    }
    fclose(file);

    write_database_into_output();
    if (shutdown(socket_fd,SHUT_WR) <0 ){
        fprintf(stderr, "Error: Failed to send FIN for colsing one end of writing from client.");
    }

    pthread_join(thread, NULL);
    
    return 0;
}