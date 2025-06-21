/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/04/12 13:27:09 by marvin           ###   ########.fr       */
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

int	parent_process_handler(t_ast *data, pid_t pid, int fd_inp, int fd_out)
{
	int	status;

	cleanup_parent_fds(fd_inp, fd_out);
	waitpid(pid, &status, 0);
	cleanup_heredoc_files(data);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
