#ifndef DATABASE_H
#define DATABASE_H
#include "course.h"
#include "student.h"
// #include "course_dll.h"
// #include "student_dll.h"
#include "dll.h"


int add_course(int roll_no,int course_code,int marks,char *response);
int delete_course(int roll_no,int course_code,char *response);
int edit_course(int roll_no,int course_code,int marks,char *response);
int add_student(int roll_no,char* name,float cgpa,char *response);
int delete_student(int roll_no,char *response);
int edit_student_cgpa(int roll_no,float cgpa,char *response);
// int edit_student_n_subjects(int roll_no,int n_subjects);
int write_into_outputfile(char *response);



#endif