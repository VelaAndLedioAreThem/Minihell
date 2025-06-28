/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 12:42:00 by user              #+#    #+#             */
/*   Updated: 2025/06/21 15:37:29 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_line(int fd, char *line, char *delim, int quoted, t_ast *data)
{
	if (!line)
	{
		ft_putstr_fd("bash: warning: here-document delimited by ",
			STDERR_FILENO);
		ft_putstr_fd("end-of-file (wanted `", STDERR_FILENO);
		ft_putstr_fd(delim, STDERR_FILENO);
		ft_putendl_fd("')", STDERR_FILENO);
		return (2);
	}
        if (ft_strcmp(line, delim) == 0)
                return (1);
        if (!quoted)
        {
                t_args arg = {.argc = g_ctx->argc - 1, .argv = g_ctx->argv + 1, .exit_status = gles(g_ctx)};
                char *expanded = parse_env(line, data->env_list, &arg);
                if (expanded)
                {
                        ft_putendl_fd(expanded, fd);
                        free(expanded);
                }
                else
                        ft_putendl_fd(line, fd);
        }
        else
                ft_putendl_fd(line, fd);
        free(line);
	return (0);
}

int	run_heredoc_loop(int fd, char *delim, int quoted, t_ast *data)
{
	char	*line;
	int		status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		status = handle_line(fd, line, delim, quoted, data);
		if (status == 1)
		{
			free(line);
			break ;
		}
		else if (status == 2)
			return (2);
	}
	return (0);
}

int	fork_heredoc(int fd, char *delim, int quoted, t_ast *data)
{
	pid_t	pid;
	int		status;
	int		ret;

	pid = fork();
	if (pid == 0)
		exit(run_heredoc_loop(fd, delim, quoted, data));
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		ret = write(STDOUT_FILENO, "\n", 1);
		(void)ret;
		return (130);
	}
	else if (WEXITSTATUS(status) != 0)
		return (1);
	return (0);
}

int	setup_heredoc_filename(t_ast *data, t_ast *node, char *tmp)
{
	free(node->right->cmd->args[0]);
	node->right->cmd->args[0] = ft_strdup(tmp);
	if (!node->right->cmd->args[0] || add_heredoc(data, ft_strdup(tmp)))
		return (1);
	return (0);
}

char	*redir_path(t_ast *n)
{
	if (!n)
		return (NULL);
	if (n->cmd && n->cmd->args && n->cmd->args[0])
		return (n->cmd->args[0]);
	return (NULL);
}
