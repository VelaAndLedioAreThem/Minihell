#include "minishell.h"
/* Execute a pipe command */
int execute_pipe(t_ast *data, t_ast *tree) {
    int fd[2];
    pid_t left_pid, right_pid;
    int left_status, right_status;

    if (pipe(fd) == -1) {
        perror("minishell: pipe");
        return (data->exit_status = 1);
    }

    // Fork for left command
    if ((left_pid = fork()) == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        exit(execute_tree(data, tree->left));
    } else if (left_pid < 0) {
        perror("minishell: fork");
        close(fd[0]);
        close(fd[1]);
        return (data->exit_status = 1);
    }

    // Fork for right command
    if ((right_pid = fork()) == 0) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        exit(execute_tree(data, tree->right));
    } else if (right_pid < 0) {
        perror("minishell: fork");
        close(fd[0]);
        close(fd[1]);
        kill(left_pid, SIGTERM); // Cleanup left child
        return (data->exit_status = 1);
    }

    // Parent process
    close(fd[0]);
    close(fd[1]);

    waitpid(left_pid, &left_status, 0);
    waitpid(right_pid, &right_status, 0);

    // Set exit status to the right child's exit code (last command)
    if (WIFEXITED(right_status))
        data->exit_status = WEXITSTATUS(right_status);
    else
        data->exit_status = 1;

    return data->exit_status;
}
