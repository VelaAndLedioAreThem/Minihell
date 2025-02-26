#include "minishell.h"
// execute_pipe.c - Updated implementation
int execute_pipe(t_data *data, t_tree *tree) {
    int fd[2];
    pid_t left_pid, right_pid;
    int status;

    if (pipe(fd) == -1)
    {
        perror("minishell: pipe");
        return (data->exit_status = 1);
    }
    
    // Left process (writes to pipe)
    if ((left_pid = fork()) == 0) {
        close(fd[0]);    // Close unused read end
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        exit(execute(data, tree->left));
    }
    
    // Right process (reads from pipe)
    if ((right_pid = fork()) == 0) {
        close(fd[1]);    // Close unused write end
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        exit(execute(data, tree->right));
    }

    // Parent process
    close(fd[0]);
    close(fd[1]);
    
    // Wait for BOTH children
    waitpid(left_pid, &status, 0);
    waitpid(right_pid, &status, 0);
    
    data->exit_status = WEXITSTATUS(status);
    return data->exit_status;
}
