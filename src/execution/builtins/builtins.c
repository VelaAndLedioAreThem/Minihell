/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:03:33 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/21 15:03:45 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	builtin_echo(t_ast *data, t_ast *tree, int fd_out)
{
	if (execute_echo(tree->cmd->args, fd_out))
		return (data->exit_status = 1, 1);
	data->exit_status = 0;
	return (1);
}

static int	builtin_exit(t_ast *data, t_ast *tree, int fd_out)
{
	(void)fd_out;
	if (execute_exit(data, tree))
		return (data->exit_status = 1, 1);
	data->exit_status = 0;
	return (1);
}

int	handle_builtin(t_ast *data, t_ast *tree, int fd_out)
{
	int						i;
	static const t_builtin	builtins[] = {{"export", builtin_export}, {"unset",
			builtin_unset}, {"env", builtin_env}, {"cd", builtin_cd}, {"pwd",
			builtin_pwd}, {"echo", builtin_echo}, {"exit", builtin_exit}, {NULL,
			NULL}};

	i = 0;
	if (!tree->cmd->args || !tree->cmd->args[0])
		return (0);
	while (builtins[i].name)
	{
		if (!ft_strcmp(tree->cmd->args[0], builtins[i].name))
			return (builtins[i].func(data, tree, fd_out));
		i++;
	}
	return (0);
}
