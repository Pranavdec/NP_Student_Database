# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -std=c99

# Source files
SOURCES = course_dll.c database.c input_processing.c student_dll.c

# Header files
HEADERS = course.h course_dll.h database.h student.h student_dll.h

# Executable name
EXECUTABLE = 2106_2122

# Objects
OBJECTS = $(SOURCES:.c=.o)

# Default rule
all: $(EXECUTABLE)

# Rule to create the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS)

# Rule to create object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

# Clean rule
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
