/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2023/00/00 00:00:00 by user              #+#    #+#             */
/*   Updated: 2023/00/00 00:00:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_ast *data, t_ast *tree, int fd_out)
{
	(void)fd_out;
	if (execute_unset(data, tree))
		return (data->exit_status = 1, 1);
	data->exit_status = 0;
	return (1);
}

int	builtin_env(t_ast *data, t_ast *tree, int fd_out)
{
	if (tree->cmd->args && tree->cmd->args[1])
	{
		ft_putstr_fd("env: ", STDERR_FILENO);
		ft_putstr_fd(tree->cmd->args[1], STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		data->exit_status = 127;
		return (1);
	}
	print_env_list(data->env_list, fd_out);
	data->exit_status = 0;
	return (1);
}
