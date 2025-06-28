/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:33:27 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/28 17:33:27 by vszpiech         ###   ########.fr       */
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
	(void)tree;
	print_env_list(data->env_list, fd_out);
	data->exit_status = 0;
	return (1);
}
