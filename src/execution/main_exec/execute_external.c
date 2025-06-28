/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/28 12:07:11 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	cleanup_parent_fds(int fd_inp, int fd_out)
{
	if (fd_inp != STDIN_FILENO)
		close(fd_inp);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
}

void	cleanup_heredoc_files(t_ast *data)
{
	int	i;

	i = -1;
	while (++i < data->heredoc_count)
	{
		unlink(data->heredoc_files[i]);
		free(data->heredoc_files[i]);
	}
	free(data->heredoc_files);
	data->heredoc_files = NULL;
	data->heredoc_count = 0;
}
