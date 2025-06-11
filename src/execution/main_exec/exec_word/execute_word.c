/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/11 08:05:25 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

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

static int	setup_fds(t_ast *data, t_ast *tree, int *fd_in, int *fd_out)
{
	(void)data;
	(void)tree;
	*fd_in = STDIN_FILENO;
	*fd_out = STDOUT_FILENO;
	return (1);
}

int	execute_word(t_ast *data, t_ast *tree)
{
	int		fd_in;
	int		fd_out;

	if (!setup_fds(data, tree, &fd_in, &fd_out))
		return (data->exit_status);
	tree->cmd->args = expand_wildcards_in_args(tree->cmd->args);
	if (handle_builtin(data, tree, fd_out))
	{
		return (data->exit_status);
	}
	fork_external_command(data, tree, fd_in, fd_out);
	return (data->exit_status);
}
