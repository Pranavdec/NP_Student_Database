#ifndef INPUT_PROCESSING_H
#define INPUT_PROCESSING_H

#include <stdio.h>

typedef struct {
    char action;
    char type;
    int category;
} QueryCategory;

typedef enum {
    Q_NONE = 0,
    Q_ADD_STUDENT,
    Q_DELETE_STUDENT,
    Q_MODIFY_STUDENT,
    Q_ADD_COURSE,
    Q_DELETE_COURSE,
    Q_MODIFY_COURSE
} QueryType;

FILE* input_processing(char* filename);
char* trim_whitespace(char* str);
int handle_student_addition(const char* line);
int handle_student_deletion(const char* line);
int handle_student_modification(const char* line);
int handle_course_addition(const char* line);
int handle_course_deletion(const char* line);
int handle_course_modification(const char* line);
QueryType categorize_query(const char* line);
int process_line(QueryType q_type, const char* line) ;
int handle_queries(char* line, FILE* file);
int process_student(FILE* file);
int parse(FILE* file);

#endif