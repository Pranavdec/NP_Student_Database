#include "student_dll.h"
#include <stdio.h>
#include <stdlib.h>

void s_insertion_beginning(Snode** head, Student student, int *rtrcode){

    Snode *ptr; 
    ptr = (Snode*)malloc(sizeof(Snode));

    if(ptr == NULL){
        // printf("Can't allocate memory for course node");
        *rtrcode = 4; //4 to show error for memmory overflow
        return;
    }else{
        ptr->student = student;
        ptr->student.sub_details = NULL;
        if(*head == NULL){
            ptr->next = NULL;
            ptr->prev = NULL;            
            *head = ptr;
            // printf("SHead is NULL\n");
        }else{
            ptr->prev = NULL;
            ptr->next = *head;
            (*head)->prev = ptr;
            *head = ptr;
            // printf("SHead is not NULL\n");
        }

        *rtrcode = 0;
        return ; //0 to show success for allocation
    }

}

void s_deletion_specified(Snode** head,Snode** sptr, int *rtrcode){

    if (*sptr == NULL) {
        *rtrcode = 5;// 5 for passing NULL value
        return;
    }

    if(*head == *sptr){
        *head = (*sptr)->next;
    }

    if((*sptr)->next != NULL){
        (*sptr)->next->prev = (*sptr)->prev;
    }

    if((*sptr)->prev != NULL){
        (*sptr)->prev->next = (*sptr)->next;
    }

    free(*sptr);
    *sptr = NULL;
    *rtrcode = 0;//Success for deletion

} 

Snode* s_search(Snode** head, int roll_no, int* rtrcode){

    // Check if the list is empty.
    if (*head == NULL) {
        *rtrcode = 1;
        return NULL;
    }

    Snode *ptr = *head;

    while(ptr != NULL){
        if((ptr->student).roll_no == roll_no){
            *rtrcode = 0;
            return ptr;
        }
        ptr = ptr->next;
    }

    // printf("No Student Found with Roll NO: %d", roll_no);
    *rtrcode = 2;//2 to show not found
    return NULL;
    

    return NULL;
} 



void s_modify(Snode** sptr,float cgpa, int * rtrcode){
    if (*sptr == NULL) {
        // fprintf(stderr, "Error: Student pointer is NULL. Cannot modify student.\n");
        *rtrcode = 5;
        return;
    }

    *rtrcode = 0;
    ((*sptr)->student).cgpa = cgpa;

}

