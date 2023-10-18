#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

int main(void)
{
    char *command = NULL;
    size_t command_length = 0;
    ssize_t read;

    while (1) {
        write(STDOUT_FILENO, "#cisfun$ ", 2);
        fflush(stdout);

        if ((read = getline(&command, &command_length, stdin)) == -1) {
            /* End of file condition (Ctrl+D) */
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        command[read - 1] = '\0';

        if (access(command, X_OK) != 0) {
            write(STDERR_FILENO, "./shell: No such file or directory", 19);
            write(STDERR_FILENO, command, strlen(command));
            write(STDERR_FILENO, "\n", 1);
            continue;
        } 
    }

    free(command);

    return 0;
}
