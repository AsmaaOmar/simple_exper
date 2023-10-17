#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024


void displayPrompt() {
    write(STDOUT_FILENO, "$ ", 2);
}
char *readCommand() {
    char *input = NULL;
    size_t input_size = 0;
    ssize_t input_length = getline(&input, &input_size, stdin);

    if (input_length == -1) {
        free(input);
        return NULL; 
    }

    input[input_length - 1] = '\0';  
    return input;
}

void executeCommand(char *command) {
    pid_t child_pid = fork();
    

    if (child_pid == -1) {
        perror("fork");
    } else if (child_pid == 0) {
        
        char *args[] = {command, NULL};
        execve(command, args, NULL);
        
        perror("execve");
        exit(1);
    } else {
     
        int status;
        if (wait(&status) == -1) {
            perror("wait");
        }
    }
}

int main() {
    char *command;

    while (1) {
        displayPrompt();  
        command = readCommand(); 

        if (command == NULL) {
            break; 
        }

        executeCommand(command); 

        free(command); 
    }

    return 0;
}
