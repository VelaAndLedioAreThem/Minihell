#include "shell.h"

void execute_pipeline(char **commands[], int num_commands) {
    int pipefd[2];
    pid_t pid;
    int in_fd;

    in_fd = STDIN_FILENO;
    int i = -1;
    while (++i < num_commands) {
        pipe(pipefd);
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            dup2(in_fd, STDIN_FILENO);
            if (i != num_commands - 1) dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
            handle_redirections(commands[i]);
            if (!handle_builtin(commands[i])) execute_command(commands[i]);
            exit(EXIT_SUCCESS);
        }
        close(pipefd[1]);
        in_fd = pipefd[0];
    }
    close(pipefd[0]);
}