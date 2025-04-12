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

static int	builtin_echo(t_ast *data, t_ast *tree, int fd_out)
{
	if (execute_echo(tree->cmd->args, fd_out))
		return (data->exit_status = 1, 1);
	return (1);
}

static int	builtin_exit(t_ast *data, t_ast *tree, int fd_out)
{
	(void)fd_out;
	if (execute_exit(data, tree))
		return (data->exit_status = 1, 1);
	return (1);
}

static const t_builtin  g_builtins[] = {
	{"export", builtin_export},
	{"unset", builtin_unset},
	{"env", builtin_env},
	{"cd", builtin_cd},
	{"pwd", builtin_pwd},
	{"echo", builtin_echo},
	{"exit", builtin_exit},
	{NULL, NULL}
	};

int	handle_builtin(t_ast *data, t_ast *tree, int fd_out)
{
	int	i;

	i = 0;
	if (!tree->cmd->args || !tree->cmd->args[0])
		return (0);
	while (g_builtins[i].name)
	{
		if (!ft_strcmp(tree->cmd->args[0], g_builtins[i].name))
			return (g_builtins[i].func(data, tree, fd_out));
		i++;
	}
	return (0);
}
