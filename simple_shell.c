#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

int main(void) {
    char command[MAX_COMMAND_LENGTH];
    int pid, status;

    while (1) {
        printf("$ ");
        fflush(stdout);

        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
            /* End of file condition (Ctrl+D) */
            printf("\n");
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0) {
            break;
        }

        if (access(command, X_OK) != 0) {
            fprintf(stderr, "Command not found: %s\n", command);
            continue;
        }

        pid = fork();

        if (pid == -1) {
            perror("fork");
            break;
        } else if (pid == 0) {
            /* Child process */
            if (execlp(command, command, NULL) == -1) {
                perror("execlp");
                _exit(EXIT_FAILURE);
            }
        } else {
            /* Parent process */
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid");
                break;
            }
        }
    }

    return 0;
}
