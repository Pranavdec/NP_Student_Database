#include "course_dll.h"
#include <stdio.h>
#include <stdlib.h>

void c_insertion_beginning(Cnode** head, Course course, int *rtrcode){

    Cnode *ptr; 
    ptr = (Cnode*)malloc(sizeof(Cnode));

    if(ptr == NULL){
        // printf("Can't allocate memory for course node");
        *rtrcode = 4; //4 to show error for memmory overflow
        return;
    }else{
        ptr->course = course;
        if(*head == NULL){
            ptr->next = NULL;
            ptr->prev = NULL;            
            *head = ptr;
            // printf("Head is NULL\n");
        }else{
            ptr->prev = NULL;
            ptr->next = *head;
            (*head)->prev = ptr;
            *head = ptr;
            // printf("Head is not NULL\n");
        }

        *rtrcode = 0;
        return ; //0 to show success for allocation
    }

}  



void c_deletion_specified(Cnode** head, Cnode** cptr, int *rtrcode){

    if (*cptr == NULL) {
        *rtrcode = 5;// 5 for passing NULL value
        return;
    }

    if(*head == *cptr){
        *head = (*cptr)->next;
    }

    if((*cptr)->next != NULL){
        (*cptr)->next->prev = (*cptr)->prev;
    }

    if((*cptr)->prev != NULL){
        (*cptr)->prev->next = (*cptr)->next;
    }

    free(*cptr);
    *cptr = NULL;
    *rtrcode = 0;//Success for deletion

}   

Cnode* c_search(Cnode** head,int course_code,int *rtrcode){

    Cnode *ptr = *head;
    if(ptr == NULL){
        *rtrcode = 1; // 1 to show database empty
        return NULL;
    }

    while(ptr != NULL){
        if((ptr->course).course_code == course_code){
            *rtrcode = 0;
            return ptr;
        }
        ptr = ptr->next;
    }

    *rtrcode = 3;//3 to show not found in course dll
    return NULL;

}

void c_modify(Cnode** cptr, int marks, int*rtrcode){

    if(*cptr == NULL){
        *rtrcode = 5;
        return;
    }

    *rtrcode = 0;
    ((*cptr)->course).marks = marks;

} 