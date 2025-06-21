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
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
	char	*arg;
	char	*end;
	long	status;
	int		count;

	count = 0;
	while (tree->cmd->args && tree->cmd->args[count])
		count++;
	if (count > 2)
	{
		ft_putendl_fd("exit: too many arguments", STDERR_FILENO);
		data->exit_status = 1;
		return (1);
	}
	status = 0;
	if (tree->cmd->args[1])
	{
		arg = tree->cmd->args[1];
		errno = 0;
		status = strtol(arg, &end, 10);
		if (*arg == '\0' || *end != '\0' || errno == ERANGE)
		{
			ft_putstr_fd("exit: ", STDERR_FILENO);
			ft_putstr_fd(arg, STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			free_data(data);
			exit(2);
		}
	}
	status %= 256;
	if (status < 0)
		status += 256;
	free_data(data);
	exit((int)status);
	return (0);
}
