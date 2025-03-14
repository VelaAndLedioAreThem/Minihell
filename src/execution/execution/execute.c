#include "minishell.h"


int execute_group(t_data *data, t_tree *tree)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        int ret = execute(data, tree->left);
        exit(ret);
    }
    else if (pid < 0)
    {
        ft_putendl_fd("minishell: fork failed", STDERR_FILENO);
        return (data->exit_status = 1);
    }
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        data->exit_status = WEXITSTATUS(status);
    else
        data->exit_status = 1;
    return data->exit_status;
}


int execute_sequence(t_data *data, t_tree *tree)
{
    execute(data, tree->left);
    return execute(data, tree->right);
}

int execute(t_data *data, t_tree *tree)
{
    if (tree->type == T_AND || tree->type == T_OR)
    {
        if (tree->type == T_AND)
            return execute_and(data, tree);
        else
            return execute_or(data, tree);
    }
    else if (tree->type == T_PIPE)
    {
        return execute_pipe(data, tree);
    }
    else if (tree->type == T_PARENTHESES)
    {
        return execute_group(data, tree);
    }
    else if (tree->type == T_SEMICOLON)
    {
        return execute_sequence(data, tree);
    }
    else if (tree->type == T_WORD)
    {
        return execute_word(data, tree);
    }
    return 0;
}
