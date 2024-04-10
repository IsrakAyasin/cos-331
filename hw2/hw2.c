#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


int main() {
    int read_bytes, pid;
    char input_buffer[80];
    char *args[5]; // Maximum of three parameters plus program name + NULL pointer

    while (1) {
        printf("⇒ "); // Prompt for user input
        fflush(stdout);
	
	// Read input from the user
	read_bytes = read(STDIN_FILENO, input_buffer, sizeof(input_buffer));

        // Parse the input command
        char *ptr = input_buffer;
        int arg_count = 0;
        int in_space = 1; // Flag to indicate if currently in space

        while (*ptr && arg_count < 4) {
            if (*ptr == ' ') {
                if (!in_space) {
                    *ptr = '\0';
                    in_space = 1;
                }
            } else {
                if (in_space) {
                    args[arg_count++] = ptr;
                    in_space = 0;
                }
            }
            ptr++;
        }

        // Null-terminate the last argument and args array
        if (*(ptr - 1) != '\0' && *(ptr - 1) != '\n') {
            *ptr = '\0';
        }

        args[arg_count] = NULL; 

        // Create a child process
        printf("Parent getting ready to fork!\n");
        pid = fork();

        if (pid == 0) { 
            printf("Child: spawning %s\n",args[0]);
            for (int i = 0; i < arg_count; i++) {
                printf("  Parameter [%d] is: %s\n", i, args[i]);
            }
            
            // Execute the command
            if (execvp(args[0], args) == -1) {
                fprintf(stderr, "Error: Command '%s' not found\n", args[0]);
                exit(EXIT_FAILURE);
            }
        } else if (pid == -1) { // Forking error
            perror("Error forking");
        } else { // Parent process
            // Wait for the child process to finish
            wait(NULL);
        }
    }
    return 0;
}
