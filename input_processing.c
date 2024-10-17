#include <stdio.h>
#include <string.h>
#include <ctype.h>
// #include "database.h"
#include "client.h"
#include "input_processing.h"


FILE* input_processing(char* filename){
    FILE* file = fopen(filename, "r");
    if(file == NULL){
        fprintf(stderr,"Error: Input File not found\n");
        return NULL;
    }
    return file;
}

char* trim_whitespace(char* str) {
    if (!str) return NULL;
    char* end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
    return str;
}

int handle_student_addition(const char* line) {    
    int roll_no, no_of_courses;
    float cgpa;
    char name[100];
    int return_code = 0;


    int fields_parsed = sscanf(line, "%d,%99[^,],%f,%d", &roll_no, name, &cgpa, &no_of_courses);
    if (fields_parsed != 4) {
        fprintf(stderr,"Error: Incorrect data format in student addition line: %s\n", line);
        return 1;
    }

    if (no_of_courses != 0) {
        fprintf(stderr,"Warning: While Student Addition, No. of Courses is not 0.\n");
    }
    // printf("Roll No: %d\nName: %s\nCGPA: %.2f\nNo of Courses: %d\n", roll_no, name, cgpa, no_of_courses);
    return_code = add_student(roll_no, name, cgpa);
    if (return_code != 0) {
        fprintf(stderr,"Error: Failed to add student with roll number %d\n", roll_no);
        return 1;
    }

    return 0;
}

int handle_student_deletion(const char* line){
    int roll_no;
    int fields_parsed = sscanf(line, "%d", &roll_no);
    if (fields_parsed != 1) {
        fprintf(stderr,"Error: Incorrect data format in student deletion line: %s\n", line);
        return 1;
    }
    // printf("Roll No: %d\n", roll_no);
    int return_code = 0;
    return_code = delete_student(roll_no);
    if (return_code != 0) {
        fprintf(stderr,"Error: Failed to delete student with roll number %d\n", roll_no);
        return 1;
    }
    return 0;
}

int handle_student_modification(const char* line){
    int roll_no;
    float cgpa;
    int fields_parsed = sscanf(line, "%d,%f", &roll_no, &cgpa);
    if (fields_parsed != 2) {
        fprintf(stderr,"Error: Incorrect data format in student modification line: %s\n", line);
        return 1;
    }
    // printf("Roll No: %d\nCGPA: %f\n", roll_no, cgpa);
    int return_code = 0;
    return_code = edit_student_cgpa(roll_no, cgpa);
    if (return_code != 0) {
        fprintf(stderr,"Error: Failed to edit student with roll number %d\n", roll_no);
        return 1;
    }
    
    return 0;
}

int handle_course_addition(const char* line){
    int roll_no, course_code, marks;
    int fields_parsed = sscanf(line, "%d,%d,%d", &roll_no, &course_code, &marks);
    if (fields_parsed != 3) {
        fprintf(stderr,"Error: Incorrect data format in course addition line: %s\n", line);
        return 1;
    }
    // printf("Roll No: %d\nCourse Code: %d\nMarks: %d\n", roll_no, course_code, marks);
    int return_code = 0;
    return_code = add_course(roll_no, course_code, marks);
    if (return_code != 0) {
        fprintf(stderr,"Error: Failed to add course with course code %d of roll_no %d\n", course_code, roll_no);
        return 1;
    }


    return 0;
}

int handle_course_deletion(const char* line){
    int roll_no, course_code;
    int fields_parsed = sscanf(line, "%d,%d", &roll_no, &course_code);
    if (fields_parsed != 2) {
        fprintf(stderr,"Error: Incorrect data format in course deletion line: %s\n", line);
        return 1;
    }
    // printf("Roll No: %d\nCourse Code: %d\n", roll_no, course_code);
    int return_code = 0;
    return_code = delete_course(roll_no, course_code);
    if (return_code != 0) {
        fprintf(stderr,"Error: Failed to delete course with course code %d of roll_no %d\n", course_code, roll_no);
        return 1;
    }
    
    return 0;
}

int handle_course_modification(const char* line){
    int roll_no, course_code, marks;
    int fields_parsed = sscanf(line, "%d,%d,%d", &roll_no, &course_code, &marks);
    if (fields_parsed != 3) {
        fprintf(stderr,"Error: Incorrect data format in course modification line: %s\n", line);
        return 1;
    }
    // printf("Roll No: %d\nCourse Code: %d\nMarks: %d\n", roll_no, course_code, marks);
    int return_code = 0;
    return_code = edit_course(roll_no, course_code, marks);
    if (return_code != 0) {
        fprintf(stderr,"Error: Failed to edit course with course code %d of roll_no %d\n", course_code, roll_no);
        return 1;
    }
    
    return 0;
}

QueryType categorize_query(const char* line) {
    if (strcmp(line, "# add student") == 0) {
        return Q_ADD_STUDENT;
    } else if (strcmp(line, "# add course") == 0) {
        return Q_ADD_COURSE;
    } else if (strcmp(line, "# modify student") == 0) {
        return Q_MODIFY_STUDENT;
    } else if (strcmp(line, "# modify course") == 0) {
        return Q_MODIFY_COURSE;
    } else if (strcmp(line, "# delete student") == 0) {
        return Q_DELETE_STUDENT;
    } else if (strcmp(line, "# delete course") == 0) {
        return Q_DELETE_COURSE;
    } else{
        return Q_NONE;
    }
}

int process_line(QueryType q_type, const char* line){
    switch (q_type){
        case Q_ADD_STUDENT:
            return handle_student_addition(line);
        case Q_DELETE_STUDENT:
            return handle_student_deletion(line);
        case Q_MODIFY_STUDENT:
            return handle_student_modification(line);
        case Q_ADD_COURSE:
            return handle_course_addition(line);
        case Q_DELETE_COURSE:
            return handle_course_deletion(line);
        case Q_MODIFY_COURSE:
            return handle_course_modification(line);
        default:
            fprintf(stderr,"Error: Unknown command type\n");
            return 1;
    }
    return 0;
}

int handle_queries(char* line, FILE* file){
    while(!feof(file)){
        int category = categorize_query(line);

        if (!fgets(line, 200, file)) {
            fprintf(stderr,"Error: Failed to read line\n");
            return 1;
        }
        while(line[0] != '#'){
            if(process_line(category, line) != 0){
                fprintf(stderr, "Error: Failed to process line :  %s\n\n", line);
            }
            if(fgets(line, 200, file)){
                continue;
            }
            else{
                return 0;
            }
        }
    }
    
    return 0;
}

int parse(FILE* file) {
    char line[200];
    char* trimmed_line;
    QueryType current_query = Q_NONE;
    
    while (fgets(line, sizeof(line), file)) {
        trimmed_line = trim_whitespace(line);

        if (strlen(trimmed_line) == 0) {
            continue;
        }

        if(trimmed_line[0] == '#'){
            current_query = categorize_query(trimmed_line);
            if (current_query == Q_NONE) {
                fprintf(stderr, "Error: Unknown command: %s\n", trimmed_line);
                return 1;
            }
            continue;
        }

        if (current_query != Q_NONE) {
            if (process_line(current_query, trimmed_line) != 0) {
                fprintf(stderr, "Error: Failed to process line: %s\n", trimmed_line);
                return 1;
            }
        } else {
            fprintf(stderr, "Error: Data line found outside of any command block: %s\n", trimmed_line);
            return 1;
        }
    }

    return 0;
}