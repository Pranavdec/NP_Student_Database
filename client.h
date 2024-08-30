#ifndef CLIENT_H
#define CLIENT_H

#define SERVER_ENDPOINT "./fifo_server"
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int initiliaze_client_fifo();
int connect_to_server();
int send_request(char *request);
int add_course(int roll_no, int course_code, int marks);
int delete_course(int roll_no, int course_code);
int edit_course(int roll_no, int course_code, int marks);
int add_student(int roll_no, char *name, float cgpa);
int delete_student(int roll_no);
int edit_student_cgpa(int roll_no, float cgpa);
int write();



#endif