#include "minishell.h"

int	execute_group(t_ast *data, t_ast *tree)
{
	pid_t	pid;
	int		status;
	int		ret;

	pid = fork();
	if (pid == 0)
	{
		ret = execute_tree(data, tree->left);
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
	return (data->exit_status);
}

int	execute_sequence(t_ast *data, t_ast *tree)
{
	execute_tree(data, tree->left);
	return (execute_tree(data, tree->right));
}

int	execute_tree(t_ast *data, t_ast *tree)
{
	if (tree->type == AST_AND || tree->type == AST_OR)
	{
		if (tree->type == AST_AND)
			return (execute_and(data, tree));
		else
			return (execute_or(data, tree));
	}
	else if (tree->type == AST_PIPELINE)
	{
		return (execute_pipe(data, tree));
	}
	else if (tree->type == AST_SUBSHELL)
	{
		return (execute_group(data, tree));
	}
	else if (tree->type == AST_COMMAND)
	{
		return (execute_word(data, tree));
	}
	return (0);
}
