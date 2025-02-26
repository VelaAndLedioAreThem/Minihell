#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

/*
 * Execute an external command by forking a child process.
 * The child sets up redirections and executes the command using execve().
 * The command executable is located using find_executable_path(), and the
 * environment is built from data->env_list via env() (as declared in your header).
 */
int execute_command(t_data *data, t_tree *tree, int fd_inp, int fd_out)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        /* Child process: set up redirections */
        if (fd_inp != STDIN_FILENO)
        {
            if (dup2(fd_inp, STDIN_FILENO) == -1)
            {
            perror("minishell: dup2 input redirection");
            exit(1);
            }
            close(fd_inp);
        }
        if (fd_out != STDOUT_FILENO)
        {
            if (dup2(fd_out, STDIN_FILENO) == -1)
            {
            perror("minishell: dup2 input redirection");
            exit(1);
            }
            close(fd_out);
        }
        /* Lookup the executable using your PATH lookup routine */
        char *cmd_path = find_executable_path(data, tree->args_array[0]);
        if (!cmd_path)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(tree->args_array[0], STDERR_FILENO);
            ft_putendl_fd(": command not found", STDERR_FILENO);
            exit(127);
        }
        if (access(cmd_path, X_OK) == -1)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd_path, STDERR_FILENO);
            ft_putendl_fd(": Permission denied", STDERR_FILENO);
            exit(126);
        }
        /* Build the environment array from data->env_list */
        char **envp = env(&(data->env_list));
        execve(cmd_path, tree->args_array, envp);
        ft_putendl_fd(strerror(errno), STDERR_FILENO);
        free_2darray(envp);  // Add helper to free string arrays
        exit(1);
    }
    else if (pid < 0)
    {
        ft_putendl_fd("minishell: fork failed", STDERR_FILENO);
        return (data->exit_status = 1);
    }
    else
    {
        /* Parent process: wait for the child and propagate its exit status */
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            data->exit_status = WEXITSTATUS(status);
        else
            data->exit_status = 1;
        return data->exit_status;
    }
}
