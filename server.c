#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "database.h"
#include "server.h"

int listen_fd;

int send_reponse(char *client_fifo_name, int query_number, char *response){
    char response_str[500];
    sprintf(response_str, "%d %s", query_number, response);

    int response_fd = open(client_fifo_name, O_WRONLY);
    write(response_fd, response_str, strlen(response_str)+1);
    close(response_fd);
}

int initiliaze_listen_fifo(){
    if((listen_fd = open(SERVER_ENDPOINT, O_RDONLY)) < 0){
        fprintf(stderr, "Error: Cannot connect to Listening FIFO\n");
        return 1;
    }

    return 0;
}

int add_student_handler(char *client_fifo_name, int query_number, char *query){
    char *temp = strtok(query,',');
    int roll_no = atoi(temp);
    char *name = strtok(query,',');
    temp = strtok(query,',');
    float cgpa = atof(temp);

    char *response[500];
    int response = add_student(roll_no, name, cgpa, response);
    return response;
}

int add_course_handler(char *client_fifo_name, int query_number, char *query){
    char *temp = strtok(query,',');
    int roll_no = atoi(temp);
    temp = strtok(query,',');
    int course_code = atoi(temp);
    temp = strtok(query,',');
    int marks = atoi(temp);

    char *response[500];
    int response = add_course(roll_no, course_code, marks, response);
    return response;
}

int modify_student_handler(char *client_fifo_name, int query_number, char *query){
    char *temp = strtok(query,',');
    int roll_no = atoi(temp);
    temp = strtok(query,',');
    float cgpa = atof(temp);

    char *response[500];
    int response = edit_student_cgpa(roll_no, cgpa, response);
    return response;
}

int modify_course_handler(char *client_fifo_name, int query_number, char *query){
    char *temp = strtok(query,',');
    int roll_no = atoi(temp);
    temp = strtok(query,',');
    int course_code = atoi(temp);
    temp = strtok(query,',');
    int marks = atoi(temp);

    char *response[500];
    int response = edit_course(roll_no, course_code, marks, response);
    return response;
}

int delete_student_handler(char *client_fifo_name, int query_number, char *query){
    char *temp = strtok(query,',');
    int roll_no = atoi(temp);

    char *response[500];
    int response = delete_student(roll_no, response);
    return response;
}

int delete_course_handler(char *client_fifo_name, int query_number, char *query){
    char *temp = strtok(query,',');
    int roll_no = atoi(temp);
    temp = strtok(query,',');
    int course_code = atoi(temp);

    char *response[500];
    int response = delete_course(roll_no, course_code, response);
    return response;
}


int categorize_send(char *client_fifo_name, int query_number, char *operation, char *query){
    if (strcmp(operation, "# add student") == 0) {
        return add_student_handler(client_fifo_name,query_number,query);
    } else if (strcmp(operation, "# add course") == 0) {
        return add_course_handler(client_fifo_name,query_number,query);
    } else if (strcmp(operation, "# modify student") == 0) {
        return modify_student_handler(client_fifo_name,query_number,query);
    } else if (strcmp(operation, "# modify course") == 0) {
        return modify_course_handler(client_fifo_name,query_number,query);
    } else if (strcmp(operation, "# delete student") == 0) {
        return delete_student_handler(client_fifo_name,query_number,query);
    } else if (strcmp(operation, "# delete course") == 0) {
        return delete_course_handler(client_fifo_name,query_number,query);
    }
    return 0;
}



int process_query(char *query){
    char *client_fio_name;
    int query_number;
    char *operation;

    client_fio_name = strtok(query, ",");
    char *temp = strtok(query,',');
    query_number = atoi(temp);

    operation = strtok(query,',');

    categorize_send(client_fio_name, query_number, operation, query);
}