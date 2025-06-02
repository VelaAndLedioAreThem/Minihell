/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/02 01:03:20 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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

int redirect_handler(t_ast *data, t_ast *tree)
{
	if (tree->type == AST_REDIR_IN)
		return (setup_input_fd(data, tree));
	else if (tree->type == AST_REDIR_OUT)
		return (setup_output_fd(data, tree));
	else if (tree->type == AST_REDIR_APPEND)
		return (setup_output_fd(data, tree));
	else if (tree->type == AST_REDIR_HERDOC)
	{
		if (create_heredoc_temp_file(data, tree))
			return (data->exit_status);
		tree->type = AST_REDIR_IN;
		return (setup_input_fd(data, tree));
	}
	else
		return (0);
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
	else if (tree->type == AST_REDIR_IN || tree->type == AST_REDIR_OUT
		|| tree->type == AST_REDIR_APPEND || tree->type == AST_REDIR_HERDOC)
	{
		redirect_handler(data, tree);
	}
	else if (tree->type == AST_COMMAND)
	{
		return (execute_word(data, tree));
	}
	return (0);
}
