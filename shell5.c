#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 10

int main() {
    while (1) {
        printf("simple_shell> ");
        char input[MAX_COMMAND_LENGTH];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nExiting the shell.\n");
            break;
        }

        size_t input_length = strlen(input);
        if (input_length > 0 && input[input_length - 1] == '\n') {
            input[input_length - 1] = '\0';  // Remove the newline character
        }

        if (strcmp(input, "exit") == 0) {
            break;
        }

        char *args[MAX_ARGS];
        int arg_count = 0;
        char *token = strtok(input, " ");
        while (token != NULL) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        // Check if the command exists in the PATH
        char *path = getenv("PATH");
        if (path != NULL) {
            char path_copy[MAX_COMMAND_LENGTH];
            strncpy(path_copy, path, sizeof(path_copy) - 1);
            path_copy[sizeof(path_copy) - 1] = '\0';

            char *path_token = strtok(path_copy, ":");
            while (path_token != NULL) {
                char command_path[MAX_COMMAND_LENGTH];
                snprintf(command_path, sizeof(command_path), "%s/%s", path_token, args[0]);
                if (access(command_path, X_OK) == 0) {
                    // Command exists in the PATH, execute it
                    pid_t pid = fork();
                    if (pid == 0) {  // Child process
                        execvp(command_path, args);
                        perror("Exec failed");
                        exit(1);
                    } else if (pid > 0) {  // Parent process
                        int status;
                        waitpid(pid, &status, 0);
                        break;
                    } else {
                        perror("Fork failed");
                    }
                }
                path_token = strtok(NULL, ":");
            }
        }

        printf("Command not found: %s\n", args[0]);
    }

    return 0;
}

