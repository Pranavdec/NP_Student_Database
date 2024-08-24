#ifndef COURSE_DLL_H
#define COURSE_DLL_H
#include "course.h"

typedef struct Cnode{
    struct Cnode *prev;
    Course course;
    struct Cnode *next;
} Cnode;

void c_insertion_beginning(Cnode** head, Course course, int* rtrcode);  
void c_deletion_specified(Cnode** head, Cnode** cptr, int* rtrcode);   
Cnode* c_search(Cnode** head, int course_code, int* rtrcode); 
void c_modify(Cnode** cptr, int marks, int *rtrcode);


#endif