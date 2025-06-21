/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vela <vela@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:54:17 by vela              #+#    #+#             */
/*   Updated: 2025/06/07 15:39:53 by vela             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>

static void	create_intermediate_outfile(char *path, int type)
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

static char	*redir_path(t_ast *n)
{
	if (!n)
		return (NULL);
	if (n->cmd && n->cmd->args && n->cmd->args[0])
		return (n->cmd->args[0]);
	return (NULL);
}

int	setup_output_fd(t_ast *data, t_ast *node)
{
	int		save;
	int		fd;
	int		flags;
	char	*path;

	path = redir_path(node->right);
	if (!path)
		return (data->exit_status = 1);
	flags = O_WRONLY | O_CREAT;
	if (node->type == AST_REDIR_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(path, flags, 0644);
	if (fd < 0)
		return (perror(path), data->exit_status = 1);
	save = dup(STDOUT_FILENO);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (close(fd), data->exit_status = 1);
	close(fd);
	data->exit_status = execute_tree(data, node->left);
	dup2(save, STDOUT_FILENO);
	close(save);
	return (data->exit_status);
}

int	create_heredoc_temp_file(t_ast *data, t_ast *node)
{
	char	tmp[sizeof(HEREDOC_TEMPLATE)];
	int		fd;
	int		status;

	fd = mkstemp(strcpy(tmp, HEREDOC_TEMPLATE));
	if (fd < 0)
		return (perror("mkstemp"), 1);
	status = fork_heredoc(fd, node->right->cmd->args[0]);
	close(fd);
	if (status != 0)
	{
		unlink(tmp);
		return (data->exit_status = status);
	}
	if (setup_heredoc_filename(data, node, tmp))
		return (data->exit_status = 1);
	return (0);
}

static t_ast	*collect_redirections(t_ast *node, char **in_file,
		char **out_file, int *out_type)
{
	t_ast	*next;
	char 	*path;

	if (!node)
		return (NULL);
	next = NULL;
	if (node->left && (node->left->type == AST_REDIR_IN
			|| node->left->type == AST_REDIR_OUT
			|| node->left->type == AST_REDIR_APPEND
			|| node->left->type == AST_REDIR_HERDOC))
		next = collect_redirections(node->left, in_file, out_file, out_type);
	else
		next = node->left;
	if (node->type == AST_REDIR_IN || node->type == AST_REDIR_HERDOC)
		*in_file = redir_path(node->right);
	else if (node->type == AST_REDIR_OUT || node->type == AST_REDIR_APPEND)
	{
		path = redir_path(node->right);
		if (*out_file)
			create_intermediate_outfile(*out_file, *out_type);
		*out_file = path;
		*out_type = node->type;
	}
	return (next);
}

static int	open_infile(char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		perror(path);
	return (fd);
}

static int	open_outfile(char *path, int type)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (type == AST_REDIR_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(path, flags, 0644);
	if (fd < 0)
		perror(path);
	return (fd);
}

int	execute_redirections(t_ast *data, t_ast *node)
{
	char	*in_file;
	char	*out_file;
	int		out_type;
	int		save_in;
	int		save_out;
	int		fd;
	t_ast	*cmd;

	in_file = NULL;
	out_file = NULL;
	out_type = AST_REDIR_OUT;
	cmd = collect_redirections(node, &in_file, &out_file, &out_type);
	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	if (in_file)
	{
		fd = open_infile(in_file);
		if (fd < 0)
			return (close(save_in), close(save_out), data->exit_status = 1);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (out_file)
	{
		fd = open_outfile(out_file, out_type);
		if (fd < 0)
		{
			if (in_file)
				dup2(save_in, STDIN_FILENO);
			close(save_in);
			close(save_out);
			return (data->exit_status = 1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	data->exit_status = execute_tree(data, cmd);
	dup2(save_in, STDIN_FILENO);
	dup2(save_out, STDOUT_FILENO);
	close(save_in);
	close(save_out);
	return (data->exit_status);
}
