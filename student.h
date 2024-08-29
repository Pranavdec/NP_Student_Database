#ifndef STUDENT_H
#define STUDENT_H
#include "dll.h"

typedef struct Student{

    int roll_no;
    char name[100];
    float cgpa;
    int n_subjects;
    node* sub_details; //head ptr to the course dll

}Student;

#endif