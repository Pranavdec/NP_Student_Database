#ifndef CLIENT_H
#define CLIENT_H
#include <semaphore.h>
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

sem_t *get_semaphore();
int initiliaze_client_fifo();
int connect_to_server();
int send_request(void *request, size_t request_size);
int add_course(int roll_no, int course_code, int marks);
int delete_course(int roll_no, int course_code);
int edit_course(int roll_no, int course_code, int marks);
int add_student(int roll_no, char *name, float cgpa);
int delete_student(int roll_no);
int edit_student_cgpa(int roll_no, float cgpa);
int write_database_into_output();



#endif