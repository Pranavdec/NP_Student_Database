#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "database.h"

typedef struct {
    char action;
    char type;
    int category;
} QueryCategory;

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

int process_subject(int roll_no, FILE* file, int no_of_courses) {
    /* Process the subjects of the student

    Args:
    roll_no: int: Roll number of the student
    file: FILE*: File pointer to the file
    no_of_courses: int: Number of courses

    Returns:
    int: 0 if successful, 1 otherwise
    */
    
    char line[200];
    int course_code, marks;
    int courses_processed = 0;

    while (courses_processed < no_of_courses) {
        if (!fgets(line, sizeof(line), file)) {
            fprintf(stderr,"Error: Failed to read line\n");
            return 1;
        }

        char* trimmed_line = trim_whitespace(line);
        if (strlen(trimmed_line) == 0) {
            continue;
        }
        int fields_processed = sscanf(trimmed_line, "%d,%d", &course_code, &marks);
        if (fields_processed != 2) {
            fprintf(stderr,"Error: Incorrect data format in course initialization line: %s\n", trimmed_line);
            return 1;
        }
        // printf("Roll np: %d\n Course Code: %d\nMarks: %d\n", roll_no, course_code, marks);
        int return_code = 0;
        return_code = add_course(roll_no, course_code, marks);
        if (return_code != 0) {
            fprintf(stderr,"Error: Failed to add course with course code %d with roll no %d\n", course_code, roll_no);
            return 1;
        }
        courses_processed++;
        
    }

    return 0;
}

int handle_student_addition(char* line) {
    char *trimmed_line = trim_whitespace(line);
    if (strlen(trimmed_line) == 0) {
        return 0;
    }
    
    int roll_no, no_of_courses;
    float cgpa;
    char name[100];
    int return_code = 0;


    int fields_parsed = sscanf(trimmed_line, "%d,%99[^,],%f,%d", &roll_no, name, &cgpa, &no_of_courses);
    if (fields_parsed != 4) {
        fprintf(stderr,"Error: Incorrect data format in student addition line: %s\n", trimmed_line);
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

int handle_student_deletion(char* line){
    char *trimmed_line = trim_whitespace(line);
    if(strlen(trimmed_line) == 0){
        return 0;
    }
    int roll_no;
    int fields_parsed = sscanf(line, "%d", &roll_no);
    if (fields_parsed != 1) {
        fprintf(stderr,"Error: Incorrect data format in student deletion line: %s\n", trimmed_line);
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

int handle_student_modification(char* line){
    char *trimmed_line = trim_whitespace(line);
    if(strlen(trimmed_line) == 0){
        return 0;
    }
    int roll_no;
    float cgpa;
    int fields_parsed = sscanf(line, "%d,%f", &roll_no, &cgpa);
    if (fields_parsed != 2) {
        fprintf(stderr,"Error: Incorrect data format in student modification line: %s\n", trimmed_line);
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

int handle_course_addition(char* line){
    char *trimmed_line = trim_whitespace(line);
    if(strlen(trimmed_line) == 0){
        return 0;
    }
    
    int roll_no, course_code, marks;
    int fields_parsed = sscanf(line, "%d,%d,%d", &roll_no, &course_code, &marks);
    if (fields_parsed != 3) {
        fprintf(stderr,"Error: Incorrect data format in course addition line: %s\n", trimmed_line);
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

int handle_course_deletion(char* line){
    char *trimmed_line = trim_whitespace(line);
    if(strlen(trimmed_line) == 0){
        return 0;
    }
    
    int roll_no, course_code;
    int fields_parsed = sscanf(line, "%d,%d", &roll_no, &course_code);
    if (fields_parsed != 2) {
        fprintf(stderr,"Error: Incorrect data format in course deletion line: %s\n", trimmed_line);
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

int handle_course_modification(char* line){
    char *trimmed_line = trim_whitespace(line);
    if(strlen(trimmed_line) == 0){
        return 0;
    }
    
    int roll_no, course_code, marks;
    int fields_parsed = sscanf(line, "%d,%d,%d", &roll_no, &course_code, &marks);
    if (fields_parsed != 3) {
        fprintf(stderr,"Error: Incorrect data format in course modification line: %s\n", trimmed_line);
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

int categorize_query(char* line) {
    char* trimmed_line = trim_whitespace(line);
    if (strlen(trimmed_line) == 0) {
        return 0;
    }

    if (strcmp(trimmed_line, "# add students") == 0) {
        return 1;
    } else if (strcmp(trimmed_line, "# add course") == 0) {
        return 4;
    } else if (strcmp(trimmed_line, "# modify student") == 0) {
        return 3;
    } else if (strcmp(trimmed_line, "# modify course") == 0) {
        return 6;
    } else if (strcmp(trimmed_line, "# delete student") == 0) {
        return 2;
    } else if (strcmp(trimmed_line, "# delete course") == 0) {
        return 5;
    }

    return 0;
}

int process_line(int category, char* line) {
    switch (category) {
        case 1:
            if (handle_student_addition(line) != 0) {
                fprintf(stderr, "Failed to handle student addition\n");
                return 1;
            }
            break;
        case 2:
            if (handle_student_deletion(line) != 0) {
                fprintf(stderr, "Failed to handle student deletion\n");
                return 1;
            }
            break;
        case 3:
            if (handle_student_modification(line) != 0) {
                fprintf(stderr, "Failed to handle student modification\n");
                return 1;
            }
            break;
        case 4:
            if (handle_course_addition(line) != 0) {
                fprintf(stderr, "Failed to handle course addition\n");
                return 1;
            }
            break;
        case 5:
            if (handle_course_deletion(line) != 0) {
                fprintf(stderr, "Failed to handle course deletion\n");
                return 1;
            }
            break;
        case 6:
            if (handle_course_modification(line) != 0) {
                fprintf(stderr, "Failed to handle course modification\n");
                return 1;
            }
            break;
        default:
            fprintf(stderr, "Error: Invalid category\n");
            return 1;
    }
    return 0;
}

int handle_queries(char* line, FILE* file){
    while(!feof(file)){
        int category = categorize_query(line);

        fgets(line, 200, file);
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

int process_student(FILE* file) {
    char line[200];
    int roll_no, no_of_courses;
    float cgpa;
    char name[100];

    while (fgets(line, sizeof(line), file) != NULL) {
        char* trimmed_line = trim_whitespace(line);

        if (strlen(trimmed_line) == 0) {
            continue;
        }

        if (trimmed_line[0] == '#') {
            break;
        }

        int fields_parsed = sscanf(trimmed_line, "%d,%99[^,],%f,%d", &roll_no, name, &cgpa, &no_of_courses);
        if (fields_parsed != 4) {
            fprintf(stderr,"Error: Incorrect data format in student initialization line: %s\n", trimmed_line);
            return 1;
        }
        // printf("Roll No: %d\nName: %s\nCGPA: %f\nNo of Courses: %d\n", roll_no, name, cgpa, no_of_courses);
        int return_code = 0;
        return_code = add_student(roll_no, name, cgpa);
        if (return_code != 0) {
            fprintf(stderr,"Error: Failed to add student with roll number %d\n", roll_no);
        }

        if (process_subject(roll_no, file, no_of_courses) != 0) {
            fprintf(stderr, "Error: Failed to process subjects for student with roll number %d\n\n", roll_no);
        }
    }

    printf("Database initialization completed\n");

    if (handle_queries(line, file) != 0) {
        fprintf(stderr,"Error: Failed to process queries\n");
        return 1;
    }
    printf("Queries processed\n");
    return 0;
}

int parse(FILE* file) {
    char line[200];
    
    while (fgets(line, sizeof(line), file)) {
        char* trimmed_line = trim_whitespace(line);

        if (strlen(trimmed_line) == 0) {
            continue;
        }

        if (trimmed_line[0] != '#' || trimmed_line[1] != ' ' || trimmed_line[2] != 'i' || trimmed_line[3] != 'n') {
            fprintf(stderr, "Error: Incorrect line for Database Initilization\n");
            return 1;
        }

        if(process_student(file) != 0){
            fprintf(stderr, "Error: Failed to process students in Database Initilization Section.\n");
            return 1;
        }
        return 0;
    }

    return 0;
}

int main(int argc, char* argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    FILE* file = input_processing(argv[1]);
    if(file == NULL){
        fprintf(stderr, "Error: Failed to open file.\n");
        return 1;
    }
    if(parse(file) != 0){
        fprintf(stderr, "Error: Failed to parse file Completely.\n");
        fclose(file);
        return 1;
    }
    fclose(file);

    write();
    
    return 0;
}