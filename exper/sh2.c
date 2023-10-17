#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_COUNT 32

int main(int argc, char *argv[]) {
    if (argc > 1) {
        // If command-line arguments are provided, execute the command directly.
        char *args[MAX_ARG_COUNT];
        int arg_count = 0;

        for (int i = 1; i < argc; i++) {
            args[arg_count] = argv[i];
            arg_count++;
        }

        args[arg_count] = NULL;

        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("fork");
        } else if (child_pid == 0) {
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(1);
            }
        } else {
            int status;
            if (wait(&status) == -1) {
                perror("wait");
            }
        }
    } else {
        // Check if standard input is a terminal (interactive mode)
        if (isatty(STDIN_FILENO)) {
            interactive_mode();
        } else {
            non_interactive_mode();
        }
    }

    return 0;
}

void interactive_mode() {
    char *input = NULL;
    size_t input_size = 0;
    char *args[MAX_ARG_COUNT];
    int arg_count = 0;
    pid_t child_pid;

    while (1) {
        write(STDOUT_FILENO, "$ ", 2);
        ssize_t input_length = getline(&input, &input_size, stdin);

        if (input_length == -1) {
            write(STDOUT_FILENO, "\n", 1);
            free(input);  // Free the input buffer before exiting
            break;
        }

        input[input_length - 1] = '\0';

        char *token = strtok(input, " ");
        arg_count = 0;  // Reset arg_count for each input line

        while (token != NULL && arg_count < MAX_ARG_COUNT - 1) {
            args[arg_count] = token;
            token = strtok(NULL, " ");
            arg_count++;
        }
        args[arg_count] = NULL;

        child_pid = fork();

        if (child_pid == -1) {
            perror("fork");
        } else if (child_pid == 0) {
            char exec_path[] = "/usr/bin/"; // Change this to the desired path
            char full_path[MAX_INPUT_SIZE]; // Adjust this buffer size as needed

            strcpy(full_path, exec_path);
            strcat(full_path, args[0]);

            char *envp[] = {NULL}; // You can set environment variables here if needed
            if (execve(full_path, args, envp) == -1) {
                perror("execve");
                exit(1);
            }
        } else {
            int status;
            if (wait(&status) == -1) {
                perror("wait");
            }
        }
    }

    free(input);
}

void non_interactive_mode() {
    
}

