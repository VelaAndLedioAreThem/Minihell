/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/00/00 00:00:00 by user              #+#    #+#             */
/*   Updated: 2023/00/00 00:00:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_export(t_ast *data, t_ast *tree, int fd_out)
{
	if (!tree->cmd->args[1])
		print_env_list(data->env_list, fd_out);
	else
		execute_export(data, tree, fd_out);
	return (1);
}

int	builtin_unset(t_ast *data, t_ast *tree, int fd_out)
{
	(void)fd_out;
	if (execute_unset(data, tree))
		return (data->exit_status = 1, 1);
	return (1);
}

int	builtin_env(t_ast *data, t_ast *tree, int fd_out)
{
	(void)tree;
	print_env_list(data->env_list, fd_out);
	return (1);
}
