/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 12:42:00 by user              #+#    #+#             */
/*   Updated: 2025/06/28 14:54:55 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*   redirection_utils.c                                                      */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

/* ---------- 1st pass : create all heredoc temp files --------------------- */

static int	handle_heredocs(t_ast *data, t_redir_ls *list)
{
	t_redir_ls	*curr;

	curr = list;
	while (curr)
	{
		if (curr->type == TOKEN_HEREDOC)
			if (!create_heredoc_file(data, curr))
				return (0);
		curr = curr->next;
	}
	return (1);
}

/* ---------- helpers to open files ---------------------------------------- */

static int	open_infiles(t_ast *data, t_redir_ls *curr, int *fd_in)
{
	int	fd;

	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	fd = open(curr->filename, O_RDONLY);
	if (fd < 0)
	{
		perror(curr->filename);
		if (data)
			data->exit_status = 1;
		return (0);
	}
	*fd_in = fd;
	return (1);
}

static int	open_outfiles(t_ast *data, t_redir_ls *curr, int *fd_out)
{
	int	fd;
	int	flags;

	if (*fd_out != STDOUT_FILENO)
		close(*fd_out);
	flags = O_WRONLY | O_CREAT;
	if (curr->type == TOKEN_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(curr->filename, flags, 0644);
	if (fd < 0)
	{
		perror(curr->filename);
		if (data)
			data->exit_status = 1;
		return (0);
	}
	*fd_out = fd;
	return (1);
}

/* ---------- 2nd pass : apply redirections -------------------------------- */

static int apply_redirs(t_ast *data, t_redir_ls *list, int *fd_in, int *fd_out)
{
	t_redir_ls	*curr;

	curr = list;
	while (curr)
	{
		if (curr->type == TOKEN_REDIRECT_IN)
		{
			if (!open_infiles(data, curr, fd_in))
				return (0);
		}
		else if (curr->type == TOKEN_REDIRECT_OUT || curr->type == TOKEN_APPEND)
		{
			if (!open_outfiles(data, curr, fd_out))
				return (0);
		}
		curr = curr->next;
	}
	return (1);
}

/* ---------- public entry -------------------------------------------------- */

int	setup_fds(t_ast *data, t_ast *tree, int *fd_in, int *fd_out)
{
	*fd_in = STDIN_FILENO;
	*fd_out = STDOUT_FILENO;
	if (!handle_heredocs(data, tree->cmd->redirections))
		return (0);
	return (apply_redirs(data, tree->cmd->redirections, fd_in, fd_out));
}
