#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
  char prompt[] = "$ ";
  char command[100];

  while (1) {
    // Display the prompt and wait for user input.
    write(STDOUT_FILENO, prompt, strlen(prompt));

    // Use getline() to read a line of input from the user.
    size_t bytes_read;
    getline(&command, &bytes_read, stdin);

    // Remove the trailing newline character from the command.
    command[bytes_read - 1] = '\0';

    // If the user presses Ctrl+D, exit the shell.
    if (command[0] == '\0') {
      break;
    }

    // Try to find the executable file for the command.
    char executable[100];
    for (int i = 0; i < bytes_read - 1; i++) {
      executable[i] = command[i];
    }
    executable[bytes_read - 1] = '\0';

    if (access(executable, X_OK)) {
      // Print an error message and display the prompt again.
      perror("execve");
      continue;
    }

    // Execute the command.
    pid_t pid = fork();
    if (pid == 0) {
      // Child process.
      execve(executable, NULL, NULL);
      exit(1);
    } else {
      // Parent process.
      int status;
      waitpid(pid, &status, 0);
    }
  }

  return 0;
}
