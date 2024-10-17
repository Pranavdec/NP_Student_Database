#ifndef GLOBALS_H
#define GLOBALS_H
#include <pthread.h>

#define SERVER_ENDPOINT "/tmp/fifo.server"
#define MAX_ATTEMPTS 3 // Maximum number of attempts to connect to server
#define RETRY_INTERVAL 5  // Time interval between two consecutive attempts to connect to server

#define SEM_NAME "/client_write_semaphore" // semaphore for client write synchronization

typedef struct IPCMESSAGE{
    char operation[20];
    int query_number;
    char response_fifo[50];
    union {
        struct {
            int arg1;
            char arg2[100];
            float arg3;
        } add_student;
        struct {
            int arg1;
            float arg2;
        } edit_student;
        struct {
            int arg1;
        } delete_student;
        struct {
            int arg1;
            int arg2;
            int arg3;
        } add_course;
        struct {
            int arg1;
            int arg2;
        } delete_course;
        struct {
            int arg1;
            int arg2;
            int arg3;
        } edit_course;
    } args;
} IPCMessage;

typedef struct CLIENTRESPONSE{
    char response[256];
    int query_number;
} CLIENTRESPONSE;

size_t readn(int fd, void *vptr, size_t n);
size_t writen(int fd, const void *vptr, size_t n);

#endif