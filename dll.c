#include "dll.h"
#include <stdio.h>
#include <stdlib.h>
#include "student.h"
#include "course.h"

void insertion_beginning(node** head, void* data, int *rtrcode){

    node *ptr; 
    ptr = (node*)malloc(sizeof(node));

    if(ptr == NULL){
        // printf("Can't allocate memory for node");
        *rtrcode = 4; //4 to show error for memmory overflow
        return;
    }else{
        ptr->data = data;
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

void deletion_specified(node** head, node** ptr, int *rtrcode){

    if (*ptr == NULL) {
        *rtrcode = 5;// 5 for passing NULL value
        return;
    }

    if(*head == *ptr){
        *head = (*ptr)->next;
    }

    if((*ptr)->next != NULL){
        (*ptr)->next->prev = (*ptr)->prev;
    }

    if((*ptr)->prev != NULL){
        (*ptr)->prev->next = (*ptr)->next;
    }

    free(*ptr);
    *ptr = NULL;
    *rtrcode = 0;//Success for deletion

}

node* search(node** head, int data, char c, int* rtrcode){
    node* ptr = *head;
    if (ptr == NULL) {
        *rtrcode = 1;
        return NULL;
    }

    if(c == 'c'){
        while(ptr != NULL){
            if(((Course*)ptr->data)->course_code == data){
                *rtrcode = 0;
                return ptr;
            }
            ptr = ptr->next;
        }
    }else if(c == 's'){
        while(ptr != NULL){
            if(((Student*)ptr->data)->roll_no == data){
                *rtrcode = 0;
                return ptr;
            }
            ptr = ptr->next;
        }
    }

    *rtrcode = 2; // 2 to show data not found
    return NULL;

}

void c_modify(node** ptr, int marks, int*rtrcode){

    if(*ptr == NULL){
        *rtrcode = 5;
        return;
    }

    ((Course*)(*ptr)->data)->marks = marks;
    *rtrcode = 0;
}

void s_modify(node** ptr, float cgpa, int * rtrcode){

    if(*ptr == NULL){
        *rtrcode = 5;
        return;
    }

    ((Student*)(*ptr)->data)->cgpa = cgpa;
    *rtrcode = 0;
}