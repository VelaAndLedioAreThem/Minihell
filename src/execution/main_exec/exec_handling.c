/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/04/10 21:21:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_and(t_ast *data, t_ast *tree)
{
	int	left_status;
	int	right_status;

	left_status = execute_tree(data, tree->left);
	if (left_status != 0)
	{
		data->exit_status = left_status;
		return (left_status);
	}
	right_status = execute_tree(data, tree->right);
	data->exit_status = right_status;
	return (right_status);
}

int	execute_or(t_ast *data, t_ast *tree)
{
	int	left_status;
	int	right_status;

	left_status = execute_tree(data, tree->left);
	if (left_status == 0)
	{
		data->exit_status = 0;
		return (0);
	}
	right_status = execute_tree(data, tree->right);
	data->exit_status = right_status;
	return (right_status);
}
