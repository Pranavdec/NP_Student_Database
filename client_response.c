#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int read_from_fifo(int fifo_fd) {
    char buffer[1024];
    ssize_t bytes_read = read(fifo_fd, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        fprintf(stderr, "Error: Cannot read from request FIFO \n");
    } else if (bytes_read == 0) {
        fprintf(stdout, "No data to read from FIFO \n");
    } else {
        buffer[bytes_read] = '\0'; // Null-terminate the string
        fprintf(stdout, "Read %ld bytes: %s\n", bytes_read, buffer);
    }

    close(fifo_fd);
}




int main() {
    int fifo_fd;

    // Open the FIFO for reading
    fifo_fd = open( "/tmp/fifoclient.9573", O_RDONLY);
    if (fifo_fd < 0) {
        fprintf(stderr, "Error: Cannot open FIFO for reading\n");
        return 1;
    }

    // Read from the FIFO and print the message
    while (1) {
        if (read_from_fifo(fifo_fd) != 0) {
            break;
        }
    }

    // Close the FIFO
    close(fifo_fd);

    return 0;
}