/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_word_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/04/12 13:14:40 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Set up input file descriptor
int	setup_input_fd(t_ast *data, t_ast *tree)
{
	int	fd_in;

	fd_in = get_input_file(data, tree);
	if (fd_in < 0)
	{
		data->exit_status = 1;
		return (-1);
	}
	return (fd_in);
}

// Set up output file descriptor
int	setup_output_fd(t_ast *data, t_ast *tree)
{
	int	fd_out;

	fd_out = get_output_file(tree);
	if (fd_out < 0)
	{
		data->exit_status = 1;
		return (-1);
	}
	return (fd_out);
}

// Close file descriptors if they're not standard
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

// Setup child process redirections
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
