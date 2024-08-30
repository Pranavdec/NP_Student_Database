#ifndef DLL_H
#define DLL_H

typedef struct node{
    struct node *prev;
    void *data;
    struct node *next;
} node;

void insertion_beginning(node** head, void* data, int* rtrcode);
void deletion_specified(node** head, node** ptr, int* rtrcode);
node* search(node** head, int data, char c, int* rtrcode);
void c_modify(node** cptr, int marks, int *rtrcode);
void s_modify(node** sptr, float cgpa, int *rtrcode);


#endif