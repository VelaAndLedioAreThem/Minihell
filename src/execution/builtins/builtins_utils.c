/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/03/25 13:17:20 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int	execute_echo(char *args[], int fd_out)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 1;
	if (args[1] && !ft_strcmp(args[1], "-n"))
	{
		n_flag = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], fd_out);
		if (args[i + 1])
			ft_putstr_fd(" ", fd_out);
		i++;
	}
	if (n_flag)
		ft_putendl_fd("", fd_out);
	return (0);
}

int	execute_exit(t_ast *data, t_ast *tree)
{
	int	status;

	status = 0;
	if (tree->cmd->args[1])
		status = ft_atoi(tree->cmd->args[1]);
	free_data(data);
	exit (status);
	return (0);
}
