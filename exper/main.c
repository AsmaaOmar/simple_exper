#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

int main() {
    char *command;

    while (1) {
        command = read(); 

        if (command == NULL) {
            break; 
        }
    }
    return 0;
}
