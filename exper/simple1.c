#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void shell() {
	char *command = NULL;
	size_t command_length = 0;
	ssize_t read;

	while (1) {
    	// Display prompt
    	write(STDOUT_FILENO, "#cisfun$ ", 10);

    	// Read command from user
    	read = getline(&command, &command_length, stdin);

    	// Handle end of file condition or read error
    	if (read == -1) {
        	break;
    	}

    	// Remove trailing newline character
    	command[strcspn(command, "\n")] = '\0';

    	// Fork a child process
    	pid_t pid = fork();

    	if (pid < 0) {
        	// Forking failed
        	write(STDERR_FILENO, "Fork failed\n", 12);
        	continue;
    	} else if (pid == 0) {
        	// Child process
        	char* args[] = {command, NULL};

        	// Execute the command
        	execvp(command, args);

        	// If execvp returns, an error occurred
        	char error_msg[100];
        	snprintf(error_msg, sizeof(error_msg), "Command not found: %s\n", command);
        	write(STDERR_FILENO, error_msg, strlen(error_msg));
        	exit(1);
    	} else {
        	// Parent process
        	// Wait for the child to terminate
        	int status;
        	waitpid(pid, &status, 0);
    	}
	}

	// Free dynamically allocated memory
	free(command);
}

int main() {
	shell();
	return 0;
}
