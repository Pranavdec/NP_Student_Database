#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "database.h"
// #include "student_dll.h"
// #include "course_dll.h"

node *head = NULL;//Student_dll head ptr


int add_course(int roll_no,int course_code,int marks){

    int dll_rtrcode = 0;

    node *ptr = search(&head, roll_no, 's', &dll_rtrcode);


    if(ptr == NULL){
        if((dll_rtrcode) == 1){
            // printf("Error 11: Student Database Empty!");
            fprintf(stderr, "Error: Student Database is Empty! Roll No: %d not found! Cannot add course\n", roll_no);
            return 1;    
        }else if((dll_rtrcode) == 2){
            // printf("Error 12: RollNo %d not found!",roll_no);
            fprintf(stderr, "Error: Roll No: %d not found! Cannot add course \n", roll_no);
            return 1;
        }
    }

    Student *sptr = (Student*)(ptr->data);
    node* cptr = search(&(sptr->sub_details), course_code, 'c', &dll_rtrcode);

    if(cptr != NULL){
        // printf("Error: Duplicate Primary Key Entry of Course Code: %d", course_code);
        fprintf(stderr, "Error: Duplicate Primary Key Entry of Course Code: %d\n", course_code);
        return 1;
    }

    Course *course = (Course *)malloc(sizeof(Course));
    course->course_code = course_code;
    course->marks = marks;

    insertion_beginning(&(sptr->sub_details),course, &dll_rtrcode);

    if(dll_rtrcode == 3){
        // printf("Error 14: Memory Overflown");
        fprintf(stderr, "Error: Memory Overflown! Cannot add course\n");
        return 1;
    }else if(dll_rtrcode == 0){
        sptr->n_subjects = sptr->n_subjects + 1;
        // printf("Success 20: Execution Successfull\n");
        return 0;
    }

    return 0;

}

int delete_course(int roll_no,int course_code){

    int dll_rtrcode = 0;

    node *ptr = search(&head, roll_no, 's', &dll_rtrcode);

    if(ptr == NULL){
        if((dll_rtrcode) == 1){
            // printf("Error 11: Student Database Empty!");
            fprintf(stderr, "Error: Student Database is Empty! Roll No: %d not found! Cannot delete course\n", roll_no);
            return 1;    
        }else if((dll_rtrcode) == 2){
            // printf("Error 12: RollNo %d not found!",roll_no);
            fprintf(stderr, "Error: Roll No: %d not found! Cannot delete course \n", roll_no);
            return 1;
        }
    }
    Student *sptr = (Student*)(ptr->data);
    node* cptr = search(&(sptr->sub_details), course_code, 'c', &dll_rtrcode);

    if(cptr == NULL){
        if((dll_rtrcode) == 1){
            // printf("Error 11: Course List Empty");
            fprintf(stderr, "Error: Course List is Empty! Cannot delete course\n");
            return 1;
        }else if((dll_rtrcode) == 3){
            // printf("Error 13: Course with Course code %d not found!", course_code);
            fprintf(stderr, "Error: Course with Course code %d not found! Cannot delete course\n", course_code);
            return 1;
        }
    }

    deletion_specified(&(sptr->sub_details),&cptr,&dll_rtrcode);

    if(dll_rtrcode == 5){
        // printf("Error 15: Passing NULL when a Course List is expected");
        fprintf(stderr, "Error: Passing NULL when a Course List is expected! Cannot delete course\n");
        return 1;
    }else{
        sptr->n_subjects = sptr->n_subjects - 1;
        // printf("Error 20: Execution Successfull\n");
        return 0;
    }

    return 0;

}

int edit_course(int roll_no,int course_code,int marks){

    int dll_rtrcode = 0;

    node *ptr = search(&head,roll_no,'s',&dll_rtrcode);

    if(ptr == NULL){
        if((dll_rtrcode) == 1){
            // printf("Error 11: Student Database Empty!");
            fprintf(stderr, "Error: Student Database is Empty! Roll No: %d not found! Cannot edit course\n", roll_no);
            return 1;    
        }else if((dll_rtrcode) == 2){
            // printf("Error 12: RollNo %d not found!",roll_no);
            fprintf(stderr, "Error: Roll No: %d not found! Cannot edit course \n", roll_no);
            return 1;
        }
    }
    Student *sptr = (Student*)(ptr->data);
    node* cptr = search(&(sptr->sub_details), course_code, 'c', &dll_rtrcode);

    if(cptr == NULL){
        if((dll_rtrcode) == 1){
            // printf("Error 11: Course List Empty");
            fprintf(stderr, "Error: Course List is Empty! Cannot edit course\n");
            return 1;
        }else if((dll_rtrcode) == 3){
            // fprintf("Error 13: Course with Course code %d not found!", course_code);
            fprintf(stderr, "Error: Course with Course code %d not found! Cannot edit course\n", course_code);
            return 1;
        }
    }

    c_modify(&cptr, marks,&dll_rtrcode);

    if(dll_rtrcode == 5){
        fprintf(stderr, "Error: Passing NULL when a Course List is expected! Cannot edit course\n");
        return 1;
    }
    // printf("Success 20: Execution Successfull\n");
    return 0;


}

int add_student(int roll_no,char* name,float cgpa){

    int dll_rtrcode = 0;

    node *sptr = search(&head,roll_no,'s',&dll_rtrcode);
    if(sptr != NULL){
        // printf("Error 21: Duplicate Primary Key Entry of Roll No: %d", roll_no);
        fprintf(stderr, "Error: Duplicate Primary Key Entry of Roll No: %d\n", roll_no);
        return 1;
    }

    Student *student = (Student *)malloc(sizeof(Student));
    if (student == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for student\n");
        return 1;
    }

    student->roll_no = roll_no;
    strncpy(student->name, name, sizeof(student->name) - 1);
    student->name[sizeof(student->name) - 1] = '\0';
    student->cgpa = cgpa;
    student->n_subjects = 0;

    insertion_beginning(&head, student, &dll_rtrcode);

    if(dll_rtrcode == 4){
        // printf("Error 14: Memory Overflown");
        fprintf(stderr, "Error: Memory Overflown! Cannot add student\n");
        return 1;
    }else if(dll_rtrcode == 0){
        // printf("Success 20: Execution Successfull\n");
        return 0;
    }
    


    return 0;
}

int delete_student(int roll_no){

    int dll_rtrcode = 0;

    node *sptr = search(&head,roll_no,'s',&dll_rtrcode);

    if(sptr == NULL){
        if((dll_rtrcode) == 1){
            // printf("Error 11: Student Database Empty!");
            fprintf(stderr, "Error: Student Database is Empty! Roll No: %d not found! Cannot delete student\n", roll_no);
            return 1;    
        }else if((dll_rtrcode) == 2){
            // printf("Error 12: RollNo %d not found!",roll_no);
            fprintf(stderr, "Error: Roll No: %d not found! Cannot delete student \n", roll_no);
            return 1;
        }
    }

    deletion_specified(&head,&sptr,&dll_rtrcode);

    if(dll_rtrcode == 5){
        // printf("Error 15: Passing NULL when a Student List is expected");
        fprintf(stderr, "Error: Passing NULL when a Student List is expected! Cannot delete student\n");
        return 1;
    }else{
        // printf("Error 20: Execution Successfull\n");
        return 0;
    }

    return 0;    

}

int edit_student_cgpa(int roll_no,float cgpa){

    int dll_rtrcode = 0;

    node *sptr = search(&head,roll_no,'s',&dll_rtrcode);

    if(sptr == NULL){
        if((dll_rtrcode) == 1){
            // printf("Error 11: Student Database Empty!");
            fprintf(stderr, "Error: Student Database is Empty! Roll No: %d not found! Cannot edit student\n", roll_no);
            return 1;    
        }else if((dll_rtrcode) == 2){
            // printf("Error 12: RollNo %d not found!",roll_no);
            fprintf(stderr, "Error: Roll No: %d not found! Cannot edit student \n", roll_no);
            return 1;
        }
    }

    s_modify(&sptr, cgpa, &dll_rtrcode);

    if(dll_rtrcode == 5){
        fprintf(stderr, "Error: Passing NULL when a Student List is expected! Cannot edit student\n");
        return 1;
    }

    // printf("Success 20: Execution Successfull");
    return 0;    

}

int write(){
    FILE *fp = fopen("2106_2122.out","w");
    if(fp == NULL){
        // printf("Error 31: File not found");
        fprintf(stderr, "Error: File not found! Cannot write to file\n");
        return 1;
    }

    node *sptr = head;
    while(sptr != NULL && sptr->next != NULL)
        sptr = sptr->next;

    while(sptr != NULL){
        Student *student = (Student*)(sptr->data);
        fprintf(fp, "%d,%s, %.2f, %d\n", student->roll_no, student->name, student->cgpa, student->n_subjects);
        node *cptr = student->sub_details;
        while(cptr != NULL && cptr->next != NULL)
                cptr = cptr->next;
        while(cptr != NULL){
            Course *course = (Course*)(cptr->data);
            fprintf(fp, "%d, %d\n", course->course_code, course->marks);
            cptr = cptr->prev;
        }
        sptr = sptr->prev;
    }


    fclose(fp);
    return 0;
}
