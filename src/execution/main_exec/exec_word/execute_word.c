/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/25 16:46:29 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fork_external_command(t_ast *data, t_ast *tree, int fd_in, int fd_out)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		ft_putendl_fd("minishell: fork failed", STDERR_FILENO);
		data->exit_status = 1;
		return (1);
	}
	if (pid == 0)
		execute_command(data, tree, fd_in, fd_out);
	else
		parent_process(pid, data);
	return (0);
}

int	create_heredoc_file(t_ast *data, t_redir_ls *redir)
{
	char	tmp[sizeof(HEREDOC_TEMPLATE)];
	int		fd;
	int		status;

	fd = open_unique_tmp(ft_strcpy(tmp, HEREDOC_TEMPLATE));
	if (fd < 0)
		return (perror("open_unique_tmp"), 0);
	status = fork_heredoc(fd, redir->filename);
	close(fd);
	if (status != 0)
	{
		unlink(tmp);
		data->exit_status = status;
		return (0);
	}
	if (add_heredoc(data, ft_strdup(tmp)))
	{
		unlink(tmp);
		data->exit_status = 1;
		return (0);
	}
	free(redir->filename);
	redir->filename = ft_strdup(tmp);
	redir->type = TOKEN_REDIRECT_IN;
	return (1);
}

int	execute_word(t_ast *data, t_ast *tree)
{
	int		fd_in;
	int		fd_out;
	char	**expanded;

	if (!setup_fds(data, tree, &fd_in, &fd_out))
		return (data->exit_status);
	expanded = expand_wildcards_in_args(tree->cmd->args);
	if (expanded)
	{
		free_2darray(tree->cmd->args);
		tree->cmd->args = expanded;
	}
	if (handle_builtin(data, tree, fd_out))
	{
		close_fds(fd_in, fd_out);
		return (data->exit_status);
	}
	fork_external_command(data, tree, fd_in, fd_out);
	close_fds(fd_in, fd_out);
	return (data->exit_status);
}

int	open_unique_tmp(char *path)
{
	static unsigned int	count;
	unsigned int		val;
	int					fd;
	int					len;
	int					i;

	len = ft_strlen(path) - 6;
	while (1)
	{
		val = ++count;
		i = 0;
		while (i < 6)
		{
			path[len + i] = "0123456789abcdef"[val & 0xf];
			val >>= 4;
			i++;
		}
		fd = open(path, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (fd >= 0 || errno != EEXIST)
			return (fd);
	}
}
