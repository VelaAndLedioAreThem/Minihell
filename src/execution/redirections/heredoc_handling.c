/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:54:17 by vela              #+#    #+#             */
/*   Updated: 2025/06/21 16:35:03 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>

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
	char	*path;

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

int	execute_redirections(t_ast *data, t_ast *node)
{
	char	*in_file;
	char	*out_file;
	int		out_type;
	int		save[2];
	t_ast	*cmd;

	in_file = NULL;
	out_file = NULL;
	out_type = AST_REDIR_OUT;
	cmd = collect_redirections(node, &in_file, &out_file, &out_type);
	save[0] = dup(STDIN_FILENO);
	save[1] = dup(STDOUT_FILENO);
	if (!redirect_input(in_file, save)
		|| !redirect_output(out_file, out_type, save, in_file != NULL))
	{
		restore_fds(save);
		return (data->exit_status = 1);
	}
	data->exit_status = execute_tree(data, cmd);
	restore_fds(save);
	return (data->exit_status);
}
