#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024


char* readCommand() {
    char* input = NULL;
    size_t input_size = 0;
    ssize_t input_length = getline(&input, &input_size, stdin);

    if (input_length == -1) {
        free(input);
        return NULL; 
    }

    input[input_length - 1] = '\0';  
    return input;
}
