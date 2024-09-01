#ifndef INPUT_PROCESSING_H
#define INPUT_PROCESSING_H

#include <stdio.h>

typedef struct {
    char action;
    char type;
    int category;
} QueryCategory;

FILE* input_processing(char* filename);
char* trim_whitespace(char* str);
int process_subject(int roll_no, FILE* file, int no_of_courses);
int handle_student_addition(char* line);
int handle_student_deletion(char* line);
int handle_student_modification(char* line);
int handle_course_addition(char* line);
int handle_course_deletion(char* line);
int handle_course_modification(char* line);
int categorize_query(char* line);
int process_line(int category, char* line);
int handle_queries(char* line, FILE* file);
int process_student(FILE* file);
int parse(FILE* file);

#endif