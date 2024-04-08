#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    int read_bytes, pid;
    char input_buffer[80];
    char *args[2];
    args[1] = NULL;

    while (1) {
        printf("⇒ ");
        fflush(stdout);

        // Read input from the user
        read_bytes = read(STDIN_FILENO, input_buffer, sizeof(input_buffer));

        if (read_bytes == -1) {
            // Error handling for file reading
            printf("Error reading input\n");
            exit(EXIT_FAILURE);
        }

        // Remove trailing newline character if present
        if (input_buffer[read_bytes - 1] == '\n') {
            input_buffer[read_bytes - 1] = '\0';
        } else {
            input_buffer[read_bytes] = '\0';
        }

        args[0] = input_buffer; // Set the first argument to the input command

        // Create a child process
        pid = fork();

        if (pid == 0) { // Child process
            // Execute the command
            execv(args[0], args);
            // Error handling for execution failure
            printf("Error executing the program\n");
            exit(EXIT_FAILURE);
        } else if (pid == -1) { // Forking error
            // Error handling for forking failure
            printf("Error forking\n");
            exit(EXIT_FAILURE);
        } else { // Parent process
            // Wait for the child process to finish
            wait(NULL);
        }
    }
    return 0;
}
