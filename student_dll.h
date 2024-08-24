#ifndef STUDENT_DLL_H
#define STUDENT_DLL_H
#include "student.h"

typedef struct Snode{
    struct Snode *prev;
    Student student;
    struct Snode *next;
}Snode;

void s_insertion_beginning(Snode** head, Student student, int * rtrcode);  
void s_deletion_specified(Snode** head,Snode** sptr, int *rtrcode);   
Snode* s_search(Snode** head, int roll_no, int * rtrcode); 
void s_modify(Snode** head, float cgpa, int * rtrcode); 

#endif