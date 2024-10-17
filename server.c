#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <pthread.h> 
#include "server.h"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int server_fd;
int server_port;
pthread_mutex_t mutex;

void handle_sigpipe() {
    fprintf(stderr, "Error: Broken pipe, client might have closed the connection\n");
    signal(SIGPIPE, handle_sigpipe);
}

int setup_server_socket() {
    struct sockaddr_in server_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        fprintf(stderr,"Error: Error during socket creation\n");
        return -1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(server_port);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        if (errno == EADDRINUSE){
            fprintf(stderr,"Error: Address already in use - produced EADDRINUSE\n");
            return -1;
        }
        fprintf(stderr,"Error: Error during bind\n");
        return -1;
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        fprintf(stderr,"Error: Error during listen\n");
        return -1;
    }

    return 0;
}

void write_response_to_socket(int client_fd, char *response, int query_number) {
    CLIENTRESPONSE client_response;
    client_response.query_number = query_number;
    strcpy(client_response.response, response);

    ssize_t bytes_written = writen(client_fd, &client_response, sizeof(CLIENTRESPONSE));
    if (bytes_written < 0) {
        if (errno == EPIPE) {
            fprintf(stderr, "Error: Client closed the connection (EPIPE)\n");
        } else {
            fprintf(stderr,"Error: Error during write into client fd\n");
        }
    }
}

void handle_request(int client_fd, IPCMessage *request) {
    if (strcmp(request->operation, "add_student") == 0) {
        handle_add_student(client_fd, request);
    } else if (strcmp(request->operation, "delete_student") == 0) {
        handle_delete_student(client_fd, request);
    } else if (strcmp(request->operation, "edit_student") == 0) {
        handle_edit_student(client_fd, request);
    } else if (strcmp(request->operation, "add_course") == 0) {
        handle_add_course(client_fd, request);
    } else if (strcmp(request->operation, "delete_course") == 0) {
        handle_delete_course(client_fd, request);
    } else if (strcmp(request->operation, "edit_course") == 0) {
        handle_edit_course(client_fd, request);
    } else if (strcmp(request->operation, "write_database") == 0) {
        handle_write_database_into_output(client_fd, request);
    } else {
        fprintf(stderr, "Error: Unknown operation %s\n", request->operation);
    }
}

void handle_add_student(int client_fd, IPCMessage *request) {
    char response[256];
    pthread_mutex_lock(&mutex);
    int result = add_student(request->args.add_student.arg1, request->args.add_student.arg2, request->args.add_student.arg3, response);
    pthread_mutex_unlock(&mutex);

    if (result == 0) {
        fprintf(stdout,"Student added successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to add student\n");
    }

    write_response_to_socket(client_fd, response, request->query_number);
}

void handle_delete_student(int client_fd, IPCMessage *request) {
    char response[256];
    pthread_mutex_lock(&mutex);
    int result = delete_student(request->args.delete_student.arg1, response);
    pthread_mutex_unlock(&mutex);

    if (result == 0) {
        fprintf(stdout,"Student deleted successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to delete student\n");
    }

    write_response_to_socket(client_fd, response, request->query_number);
}

void handle_edit_student(int client_fd, IPCMessage *request) {
    char response[256];
    pthread_mutex_lock(&mutex);
    int result = edit_student_cgpa(request->args.edit_student.arg1, request->args.edit_student.arg2, response);
    pthread_mutex_unlock(&mutex);

    if (result == 0) {
        fprintf(stdout,"Student edited successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to edit student\n");
    }

    write_response_to_socket(client_fd, response, request->query_number);
}

void handle_add_course(int client_fd, IPCMessage *request) {
    char response[256];
    pthread_mutex_lock(&mutex);
    int result = add_course(request->args.add_course.arg1, request->args.add_course.arg2, request->args.add_course.arg3, response);
    pthread_mutex_unlock(&mutex);

    if (result == 0) {
        fprintf(stdout,"Course added successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to add course\n");
    }

    write_response_to_socket(client_fd, response, request->query_number);
}

void handle_delete_course(int client_fd, IPCMessage *request) {
    char response[256];
    pthread_mutex_lock(&mutex);
    int result = delete_course(request->args.delete_course.arg1, request->args.delete_course.arg2, response);
    pthread_mutex_unlock(&mutex);

    if (result == 0) {
        fprintf(stdout,"Course deleted successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to delete course\n");
    }

    write_response_to_socket(client_fd, response, request->query_number);
}

void handle_edit_course(int client_fd, IPCMessage *request) {
    char response[256];
    pthread_mutex_lock(&mutex);
    int result = edit_course(request->args.edit_course.arg1, request->args.edit_course.arg2, request->args.edit_course.arg3, response);
    pthread_mutex_unlock(&mutex);

    if (result == 0) {
        fprintf(stdout,"Course edited successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to edit course\n");
    }

    write_response_to_socket(client_fd, response, request->query_number);
}

void handle_write_database_into_output(int client_fd, IPCMessage *request) {
    char response[256];
    pthread_mutex_lock(&mutex);
    int result = write_database_into_output(response);
    pthread_mutex_unlock(&mutex);

    if (result == 0) {
        fprintf(stdout,"Database written to output successfully\n");
    } else {
        fprintf(stderr, "Error: Failed to write database to output\n");
    }

    write_response_to_socket(client_fd, response, request->query_number);
}

void handle_client(int client_fd) {
    IPCMessage request;
    ssize_t bytes_read;

    while ((bytes_read = readn(client_fd, &request, sizeof(IPCMessage))) > 0) {
        if (bytes_read == sizeof(IPCMessage)){
            handle_request(client_fd, &request);
        }else{
            fprintf(stderr, "Error: Client sent incomplete IPCMessage when querying\n");
        }
    }

    if (bytes_read == 0) {
        fprintf(stdout,"Client disconnected\n");
    } else if (bytes_read < 0) {
        fprintf(stderr,"Error: Cannot read from socket\n");
    }

    close(client_fd);
}

void *client_handler(void *arg) {
    int client_fd = *((int *)arg);
    free(arg);
    handle_client(client_fd);

    return NULL;
}

int main(int argc, char *argv[]) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_mutex_init(&mutex, NULL);

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    server_port = atoi(argv[1]);
    if (server_port <= 0 || server_port > 65535) {
        fprintf(stderr, "Error: Invalid port number. Please provide a port between 1 and 65535.\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGPIPE, handle_sigpipe);

    if (setup_server_socket() < 0) {
        exit(EXIT_FAILURE);
    }

    fprintf(stdout,"Server is running and waiting for client connections on port %d...\n", server_port);

    int *client_fd;

    while (1) {
        client_fd = malloc(sizeof(int));

        *client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (*client_fd < 0) {
            fprintf(stderr,"Error: Cannot accept client connection\n");
            free(client_fd);
            continue;
        }

        fprintf(stdout,"Accepted connection from client\n");

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, client_handler, client_fd) != 0) {
            fprintf(stderr,"Error: Cannot create thread to process client. Client connection closing..\n");
            close(*client_fd);  
            free(client_fd);
            continue;
        }

        pthread_detach(thread_id);
    }

    pthread_mutex_destroy(&mutex);
    close(server_fd);
    return 0;
}
