/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 12:42:00 by user              #+#    #+#             */
/*   Updated: 2025/04/07 15:39:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void	print_error(char *filename, char *error_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(error_msg, STDERR_FILENO);
}

static int	open_regular_input(char *last_red_inp)
{
	int	fd;

	fd = open(last_red_inp, O_RDONLY);
	if (fd < 0)
	{
		print_error(last_red_inp, strerror(errno));
		return (-1);
	}
	return (fd);
}
/* ==================== INPUT FILE HANDLING ==================== */

static void	handle_redir_in(t_ast *node, char **last_red_inp)
{
	if (node->type == AST_REDIR_IN)
		*last_red_inp = node->token->value;
}

static int	hh(t_ast *node, t_ast *data, char **delim, int *fd)
{
	if (node->type == AST_EOF)
	{
		*delim = node->token->value;
		*fd = create_heredoc_temp_file(*delim, data);
		if (*fd < 0)
			return (-1);
	}
	return (0);
}

int	get_input_file(t_ast *data, t_ast *tree)
{
	t_ast	*curr;
	char	*last_red_inp;
	char	*lhd;
	int		fd;

	curr = tree;
	last_red_inp = NULL;
	lhd = NULL;
	fd = STDIN_FILENO;
	while (curr)
	{
		handle_redir_in(curr, &last_red_inp);
		if (curr->type == AST_EOF && hh(curr, data, &lhd, &fd) == -1)
			return (-1);
		if (curr->left)
		{
			fd = get_input_file(data, curr->left);
			if (fd == -1)
				return (-1);
		}
		curr = curr->right;
	}
	if (!lhd && last_red_inp)
		fd = open_regular_input(last_red_inp);
	return (fd);
}

static void	find_last_output_redirection(t_ast *curr, char **file, int *type)
{
	while (curr)
	{
		if (curr->type == AST_REDIR_OUT || curr->type == AST_REDIR_APPEND)
		{
			*file = curr->token->value;
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
