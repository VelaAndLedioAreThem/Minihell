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

/* Return the filename that follows a redirection, or NULL */
static char	*redir_path(t_ast *n)
{
	if (!n)
		return (NULL);
	if (n->cmd && n->cmd->args && n->cmd->args[0])
		return (n->cmd->args[0]);
	return (NULL);
}

int	setup_input_fd(t_ast *data, t_ast *node)
{
	int		save;
	int		fd;
	char	*path;

	path = redir_path(node->right);
	if (!path)
		return (data->exit_status = 1);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (perror(path), data->exit_status = 1);
	save = dup(STDIN_FILENO);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (close(fd), data->exit_status = 1);
	close(fd);
	data->exit_status = execute_tree(data, node->left);
	dup2(save, STDIN_FILENO);
	close(save);
	return (data->exit_status);
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
