/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 12:42:00 by user              #+#    #+#             */
/*   Updated: 2025/06/21 15:37:17 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	print_error(char *filename, char *error_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(error_msg, STDERR_FILENO);
}

static void	find_last_output_redirection(t_ast *curr, char **file, int *type)
{
	while (curr)
	{
		if (curr->type == AST_REDIR_OUT || curr->type == AST_REDIR_APPEND)
		{
			*file = curr->cmd->args[0];
			*type = curr->type;
		}
		curr = curr->right;
	}
}

static int	open_output_file(char *filename, int redir_type)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (redir_type == AST_REDIR_OUT)
		flags |= O_TRUNC;
	else if (redir_type == AST_REDIR_APPEND)
		flags |= O_APPEND;
	fd = open(filename, flags, 0644);
	if (fd < 0)
		print_error(filename, strerror(errno));
	return (fd);
}

int	get_output_file(t_ast *tree)
{
	char	*last_file;
	int		redir_type;
	int		fd;

	last_file = NULL;
	redir_type = -1;
	fd = STDOUT_FILENO;
	find_last_output_redirection(tree->right, &last_file, &redir_type);
	if (last_file)
	{
		fd = open_output_file(last_file, redir_type);
		if (fd < 0)
			return (-1);
	}
	return (fd);
}
void	create_intermediate_outfile(char *path, int type)
{
	int	flags;
	int	fd;

	if (!path)
		return ;
	flags = O_WRONLY | O_CREAT;
	if (type == AST_REDIR_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(path, flags, 0644);
	if (fd >= 0)
		close(fd);
	else
		perror(path);
}
