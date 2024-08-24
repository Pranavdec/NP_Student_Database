#ifndef DATABASE_H
#define DATABASE_H
#include "course.h"
#include "student.h"
#include "course_dll.h"
#include "student_dll.h"


int add_course(int roll_no,int course_code,int marks);
int delete_course(int roll_no,int course_code);
int edit_course(int roll_no,int course_code,int marks);
int add_student(int roll_no,char* name,float cgpa);
int delete_student(int roll_no);
int edit_student_cgpa(int roll_no,float cgpa);
// int edit_student_n_subjects(int roll_no,int n_subjects);
int write();



#endif