/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/04/10 21:42:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Fork new process for external command
int	fork_external_command(t_ast *data, t_ast *tree, int fd_in, int fd_out)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		ft_putendl_fd("minishell: fork failed", STDERR_FILENO);
		data->exit_status = 1;
		return (1);
	}
	if (pid == 0)
		execute_command(data, tree, fd_in, fd_out);
	else
		parent_process(pid, data);
	return (0);
}

// Function to set up file descriptors and validate them
static int	setup_fds(t_ast *data, t_ast *tree, int *fd_in, int *fd_out)
{
	*fd_in = setup_input_fd(data, tree);
	*fd_out = setup_output_fd(data, tree);
	if (*fd_in < 0 || *fd_out < 0)
	{
		close_fds(*fd_in, *fd_out);
		return (0);
	}
	return (1);
}

// Function to handle argument expansion and validation
static char	**prepare_args(t_ast *data, t_ast *tree, int fd_in, int fd_out)
{
	char	**expanded_args;

	expanded_args = expand_args(tree->cmd->args);
	if (!expanded_args)
	{
		close_fds(fd_in, fd_out);
		data->exit_status = 1;
	}
	return (expanded_args);
}

// Function to swap command arguments
static void	swap_args(t_ast *tree, char **new_args, char ***old_args)
{
	*old_args = tree->cmd->args;
	tree->cmd->args = new_args;
}

// Main function that orchestrates the execution
int	execute_word(t_ast *data, t_ast *tree)
{
	int		fd_in;
	int		fd_out;
	char	**expanded_args;
	char	**original_args;

	if (!setup_fds(data, tree, &fd_in, &fd_out))
		return (data->exit_status);
	expanded_args = prepare_args(data, tree, fd_in, fd_out);
	if (!expanded_args)
		return (data->exit_status);
	swap_args(tree, expanded_args, &original_args);
	if (handle_builtin(data, tree, fd_out))
	{
		tree->cmd->args = original_args;
		cleanup_resources(fd_in, fd_out, expanded_args);
		return (data->exit_status);
	}
	fork_external_command(data, tree, fd_in, fd_out);
	tree->cmd->args = original_args;
	cleanup_resources(fd_in, fd_out, expanded_args);
	return (data->exit_status);
}
