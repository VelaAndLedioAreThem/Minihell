/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/28 12:04:55 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	signal_child(void)
{
	kill(g_ctx->child_pid, SIGINT);
	g_ctx->child_pid++;
}

void	handle_sigint(int signo)
{
	int	i;

	if (signo != SIGINT)
		return ;
	if (!isatty(STDIN_FILENO))
		exit(EXIT_SUCCESS);
	i = write(1, "\n", 1);
	(void)i;
	if (g_ctx->child_pid == 42)
		g_ctx->child_pid = 44;
	if (g_ctx->child_pid != 0 && g_ctx->child_pid != 44)
		signal_child();
	else
		redisplay_prompt();
}

void	handle_sigtstp_sigquit(int signo)
{
	if (signo == SIGTSTP || signo == SIGQUIT)
	{
		if (isatty(STDIN_FILENO))
		{
			if (g_ctx->child_pid == 0)
				rl_redisplay();
			else
			{
				kill(g_ctx->child_pid, signo);
				g_ctx->child_pid += 2;
			}
		}
	}
}
