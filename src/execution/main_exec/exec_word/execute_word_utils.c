/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_word_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:34:08 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/28 17:34:10 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
}

void	cleanup_resources(int fd_in, int fd_out, char **expanded_args)
{
	free_2darray(expanded_args);
	close_fds(fd_in, fd_out);
}

void	setup_child_redirections(int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (fd_out != STDOUT_FILENO)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
}
