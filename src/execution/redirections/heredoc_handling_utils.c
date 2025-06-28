/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handling_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vela <vela@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:05:56 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/28 22:37:07 by vela             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>

void	restore_fds(int *save)
{
	dup2(save[0], STDIN_FILENO);
	dup2(save[1], STDOUT_FILENO);
	close(save[0]);
	close(save[1]);
}

int	redirect_input(char *file, int *save)
{
	int	fd;

	(void)save;
	if (!file)
		return (1);
	fd = open_infile(file);
	if (fd < 0)
		return (0);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

int	redirect_output(char *file, int type, int *save, int has_in)
{
	int	fd;

	if (!file)
		return (1);
	fd = open_outfile(file, type);
	if (fd < 0)
	{
		if (has_in)
			dup2(save[0], STDIN_FILENO);
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

char	*redir_path(t_ast *n)
{
	if (!n)
		return (NULL);
	if (n->cmd && n->cmd->args && n->cmd->args[0])
		return (n->cmd->args[0]);
	return (NULL);
}
