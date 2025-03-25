#include "minishell.h"

int execute_word(t_ast *data, t_ast *tree)
{
    
    // Get input and output file descriptors
    int fd_in = get_input_file(data, tree);
    int fd_out = get_output_file(tree);
    
    if (fd_in < 0 || fd_out < 0)
    {
        if (fd_in > 0 && fd_in != STDIN_FILENO)
            close(fd_in);
        if (fd_out > 0 && fd_out != STDOUT_FILENO)
            close(fd_out);
        data->exit_status = 1;
        return 1;
    }

    char **expanded_args = expand_wildcards_in_args(tree->cmd->args);
    if (!expanded_args) {
        close(fd_in);
        close(fd_out);
        return (data->exit_status = 1);
    }

    char **original_args = tree->cmd->args;
    tree->cmd->args = expanded_args;
    // Try to execute as builtin
    if (handle_builtin(data, tree, fd_out))
    {
        tree->cmd->args = original_args;
        free_2darray(expanded_args);
        if (fd_in != STDIN_FILENO)
            close(fd_in);
        if (fd_out != STDOUT_FILENO)
            close(fd_out);
        return data->exit_status;
    }
    
    // Execute as external command
    pid_t pid = fork();
    if (pid < 0)
    {
        ft_putendl_fd("minishell: fork failed", STDERR_FILENO);
        data->exit_status = 1;
    }
    if (pid == 0)
    {
        // Child process
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        
        // Set up redirections
        if (fd_in != STDIN_FILENO)
        {
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }
        if (fd_out != STDOUT_FILENO)
        {
            dup2(fd_out, STDOUT_FILENO);
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
        
        // If execve returns, there was an error
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putendl_fd(strerror(errno), STDERR_FILENO);
        free(cmd_path);
        free_2darray(envp);
        exit(126);
    } 
    else
    {
        // Parent process
        int status;
        g_child_pid = pid;
        waitpid(pid, &status, 0);
        g_child_pid = 0; // Reset after child exi
        if (WIFEXITED(status))
            data->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            data->exit_status = 128 + WTERMSIG(status);
        else
            data->exit_status = 1;
    }
    tree->cmd->args = original_args;
    free_2darray(expanded_args);
    // Clean up file descriptors
    if (fd_in != STDIN_FILENO)
        close(fd_in);
    if (fd_out != STDOUT_FILENO)
        close(fd_out);
    
    return data->exit_status;
}
