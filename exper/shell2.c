#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_COUNT 32

int main() {
    char *input = NULL;
    size_t input_size = 0;
    char *args[MAX_ARG_COUNT];
    int arg_count = 0;
    ssize_t input_length;
    pid_t child_pid;
    char *token;
    

    while (1) {
	    char *envp[] = {"/usr/bin/"};

        write(STDOUT_FILENO, "$ ", 2);
        input_length = getline(&input, &input_size, stdin);

        if (input_length == -1) {
            write(STDOUT_FILENO, "\n", 1);
            
           
	    break;
	    free(input);
        }

        input[input_length - 1] = '\0';

        token = strtok(input, " ");
        arg_count = 0;  

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
            char *exec_path = "/usr/bin/"; 
            char full_path[1024]; 
            snprintf(full_path, sizeof(full_path), "%s%s", exec_path, args[0]);
             
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
    return 0;
}
