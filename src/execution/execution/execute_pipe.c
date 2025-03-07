#include "minishell.h"
// execute_pipe.c - Updated implementation
int execute_pipe(t_data *data, t_tree *tree) {
    int fd[2];
    pid_t left_pid, right_pid;
    int left_status, right_status;

    if (pipe(fd) == -1) {
        perror("minishell: pipe");
        return (data->exit_status = 1);
    }

    // Left process (writes to pipe)
    if ((left_pid = fork()) == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        exit(execute(data, tree->left));
    }

    // Right process (reads from pipe)
    if ((right_pid = fork()) == 0) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        exit(execute(data, tree->right));
    }

    // Parent closes pipe and waits
    close(fd[0]);
    close(fd[1]);
    waitpid(left_pid, &left_status, 0);
    waitpid(right_pid, &right_status, 0);

    // Set exit status to the rightmost command's status
    if (WIFEXITED(right_status))
        data->exit_status = WEXITSTATUS(right_status);
    else
        data->exit_status = 1;

    return data->exit_status;
}