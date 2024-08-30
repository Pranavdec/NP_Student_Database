#include <stdio.h>
#include <string.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "client.h"

char *client_fifo;
int server_fd;
int request_id = 0;

int initiliaze_client_fifo(){
    pid_t pid = getpid();
    client_fifo = (char *)malloc(20);
    sprintf(client_fifo, "./fifo_client_%d", pid);
    if(mkfifo(client_fifo, FILE_MODE) < 0){
        fprintf(stderr, "Error: Cannot create client fifo\n");
        return 1;
    }
    return 0;
}

int connect_to_server(){
    if((server_fd = open(SERVER_ENDPOINT, O_WRONLY)) < 0){
        fprintf(stderr, "Error: Cannot connect to server\n");
        return 1;
    }
}

int send_request(char *request){
    if(write(server_fd, request, strlen(request)) < 0){
        fprintf(stderr, "Error: Cannot send request to server\n");
        return 1;
    }
    return 0;
}

int add_course(int roll_no, int course_code, int marks){
    char request[200];
    sprintf(request, "%s,%d,add_course,%d,%d,%d", client_fifo, request_id, roll_no, course_code, marks);
    request_id++;
    if(send_request(request) == 1){
        return 1;
    }
    return 0;
}

int delete_course(int roll_no, int course_code){
    char request[200];
    sprintf(request, "%s,%d,delete_course,%d,%d", client_fifo, request_id, roll_no, course_code);
    request_id++;
    if(send_request(request) == 1){
        return 1;
    }
    return 0;
}

int edit_course(int roll_no, int course_code, int marks){
    char request[200];
    sprintf(request, "%s,%d,edit_course,%d,%d,%d", client_fifo, request_id, roll_no, course_code, marks);
    request_id++;
    if(send_request(request) == 1){
        return 1;
    }
    return 0;
}

int add_student(int roll_no, char *name, float cgpa){
    char request[200];
    sprintf(request, "%s,%d,add_student,%d,%s,%f", client_fifo, request_id, roll_no, name, cgpa);
    request_id++;
    if(send_request(request) == 1){
        return 1;
    }
    return 0;
}

int delete_student(int roll_no){
    char request[200];
    sprintf(request, "%s,%d,delete_student,%d", client_fifo, request_id, roll_no);
    request_id++;
    if(send_request(request) == 1){
        return 1;
    }
    return 0;
}

int edit_student_cgpa(int roll_no, char *name, float cgpa){
    char request[200];
    sprintf(request, "%s,%d,edit_student,%d,%s,%f", client_fifo, request_id, roll_no, name, cgpa);
    request_id++;
    if(send_request(request) == 1){
        return 1;
    }
    return 0;
}

int write_into_outputfile(){
    char request[200];
    sprintf(request, "%s,%d,write", client_fifo, request_id);
    if(send_request(request) == 1){
        return 1;
    }
    return 0;
}