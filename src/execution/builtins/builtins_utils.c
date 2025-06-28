/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/28 14:28:30 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int     execute_echo(char *args[], int fd_out)
{
    int		i;
    int		n_flag;

    i = 1;
    n_flag = 1;
    while (args[i] && args[i][0] == '-' && args[i][1])
    {
        int j = 1;
        while (args[i][j] == 'n')
            j++;
        if (args[i][j] != '\0')
            break;
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
    else
        ft_putstr_fd("", fd_out);
    return (0);
}

static int	too_many_args(t_ast *data)
{
	ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
	data->exit_status = 1;
	return (1);
}

static void	handle_non_numeric(t_ast *data, char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	free_data(data);
	exit(2);
}

int	execute_exit(t_ast *data, t_ast *tree)
{
	long	status;
	char	*end;
	int		count;

	count = 0;
	while (tree->cmd->args && tree->cmd->args[count])
		count++;
	if (count > 2)
		return (too_many_args(data));
	errno = 0;
	status = data->exit_status;
	if (tree->cmd->args[1])
	{
		status = strtol(tree->cmd->args[1], &end, 10);
		if (*(tree->cmd->args[1]) == '\0' || *end != '\0' || errno == ERANGE)
			handle_non_numeric(data, tree->cmd->args[1]);
	}
	status %= 256;
	if (status < 0)
		status += 256;
	free_data(data);
	exit((int)status);
	return (0);
}
