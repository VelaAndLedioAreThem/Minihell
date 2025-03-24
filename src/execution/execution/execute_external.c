#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

/* Execute an external command with proper redirections */
int execute_command(t_ast *data, t_ast *tree, int fd_inp, int fd_out)
{
    pid_t pid;
    int status;


    pid = fork();
    if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        
        // Handle input redirection
        if (fd_inp != STDIN_FILENO) {
            if (dup2(fd_inp, STDOUT_FILENO) == -1) {
                perror("minishell: dup2 input redirection");
                exit(1);
            }
            close(fd_inp);
        }
        
        // Handle output redirection
        if (fd_out != STDOUT_FILENO) {
            if (dup2(fd_out, STDOUT_FILENO) == -1) {
                perror("minishell: dup2 output redirection");
                exit(1);
            }
            close(fd_out);
        }
        
        // Execute command
        char *cmd_path = find_executable_path(data, tree->cmd->args[0]);
        if (!cmd_path) {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(tree->cmd->args[0], STDERR_FILENO);
            ft_putendl_fd(": command not found", STDERR_FILENO);
            exit(127);
        }
        
        char **envp = env(&(data->env_list));
        execve(cmd_path, tree->cmd->args, envp);
        perror("minishell: execve");
        free_2darray(envp);
        exit(1);
    }
    
    // Parent process
    if (fd_inp != STDIN_FILENO)
        close(fd_inp);
    if (fd_out != STDOUT_FILENO)
        close(fd_out);
        
    waitpid(pid, &status, 0);
    
    // Cleanup heredoc files
    for (int i = 0; i < data->heredoc_count; i++) {
        unlink(data->heredoc_files[i]);
        free(data->heredoc_files[i]);
    }
    free(data->heredoc_files);
    data->heredoc_files = NULL;
    data->heredoc_count = 0;
    
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return 1;
}
