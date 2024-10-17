# IPC Server-Client System

## Setup and Compilation

1. Unzip the Tar

2. To compile the program, simply run the following command:

    ```bash
    make
    ```

   This will generate two executable files:

    - `server`
    - `client`

## Running the Program

### Running the Server

To start the server, use the following command:

```bash
./server
```

The server will begin running, waiting for client requests.

### Running a Client

To start a client, use the following command:

```bash
./client <file_input>
```

For example, to run the client with an input file named `input.txt`:

```bash
./client input.txt
```

### Client Input

Each client's input file should start with a `# initial database` line.

### Writing Database State

Once a client has finished sending all of its required requests, it will send a final request to write the current database state to a text file on the server side which will be written into the file 2106_2122.out

## Notes

- The server supports multiple clients simultaneously.
- Each client will receive responses from the server in real-time.
- Ensure that the input file for each client starts with `# initial database`, denoting the initial database setup.
- Server doesn't exit unless a gracefull exit occurs.