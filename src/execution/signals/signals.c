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

void	handle_sigint(int signo)
{
	int	i;

	if (signo == SIGINT)
	{
		if (isatty(STDIN_FILENO))
		{
			i = write(1, "\n", 1);
			(void) i;
			if (g_child_pid == 42)
				g_child_pid = 44;
			if (g_child_pid != 0 && g_child_pid != 44)
			{
				kill(g_child_pid, SIGINT);
				g_child_pid++;
			}
			else
			{
				rl_on_new_line();
				rl_redisplay();
			}
		}
		else
			exit(EXIT_SUCCESS);
	}
}

void	handle_sigtstp_sigquit(int signo)
{
	if (signo == SIGTSTP || signo == SIGQUIT)
	{
		if (isatty(STDIN_FILENO))
		{
			if (g_child_pid == 0)
				rl_redisplay();
			else
			{
				kill(g_child_pid, signo);
				g_child_pid += 2;
			}
		}
	}
}

void	handle_c(int signo)
{
	handle_sigint(signo);
	handle_sigtstp_sigquit(signo);
}

int	handle_d(t_ast *data, char *line)
{
	(void)data;
	if (line == NULL)
	{
		rl_on_new_line();
		rl_redisplay();
	}
	if (ft_strlen(line) == 0)
	{
		ft_strdel(&line);
		return (1);
	}
	return (0);
}
