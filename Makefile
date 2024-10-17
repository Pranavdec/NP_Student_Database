CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c99

SERVER = server
CLIENT = client

SERVER_SOURCES = server.c database.c dll.c
CLIENT_SOURCES = client.c input_processing.c globals.c

HEADERS = course.h dll.h database.h student.h globals.h

SERVER_OBJECTS = $(SERVER_SOURCES:.c=.o)
CLIENT_OBJECTS = $(CLIENT_SOURCES:.c=.o)


all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_OBJECTS)
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER_OBJECTS)

$(CLIENT): $(CLIENT_OBJECTS)
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT_OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(SERVER_OBJECTS) $(CLIENT_OBJECTS) $(SERVER) $(CLIENT)
	rm -f 2106_2122.out
