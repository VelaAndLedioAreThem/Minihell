/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/21 16:14:45 by ldurmish         ###   ########.fr       */
/*   Updated: 2025/06/21 16:04:48 by ldurmish         ###   ########.fr       */
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
        char    tmp[sizeof(HEREDOC_TEMPLATE)];
        int             fd;
        int             status;

        fd = mkstemp(strcpy(tmp, HEREDOC_TEMPLATE));
        if (fd < 0)
                return (perror("mkstemp"), 0);
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

int      setup_fds(t_ast *data, t_ast *tree, int *fd_in, int *fd_out)
{
        t_redir_ls		*curr;
        int             fd;
        int             flags;

        *fd_in = STDIN_FILENO;
        *fd_out = STDOUT_FILENO;
        curr = tree->cmd->redirections;
        while (curr)
        {
			if (curr->type == TOKEN_HEREDOC)
			{
					if (!create_heredoc_file(data, curr))
							return (0);
			}
			curr = curr->next;
        }
        curr = tree->cmd->redirections;
        while (curr)
        {
                if (curr->type == TOKEN_REDIRECT_IN)
                {
                        if (*fd_in != STDIN_FILENO)
                                close(*fd_in);
                        fd = open(curr->filename, O_RDONLY);
                        if (fd < 0)
                                return (perror(curr->filename), 0);
                        *fd_in = fd;
                }
                else if (curr->type == TOKEN_REDIRECT_OUT ||
                                curr->type == TOKEN_APPEND)
                {
                        if (*fd_out != STDOUT_FILENO)
                                close(*fd_out);
                        flags = O_WRONLY | O_CREAT;
                        if (curr->type == TOKEN_APPEND)
                                flags |= O_APPEND;
                        else
                                flags |= O_TRUNC;
                        fd = open(curr->filename, flags, 0644);
                        if (fd < 0)
                                return (perror(curr->filename), 0);
                        *fd_out = fd;
                }
                curr = curr->next;
        }
        return (1);
}

{
	int				fd_in;
	int				fd_out;
	char			**expanded;

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
