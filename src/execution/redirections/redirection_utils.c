/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vela <vela@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 12:42:00 by user              #+#    #+#             */
/*   Updated: 2025/06/29 13:01:07 by vela             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
static char     *read_line_fd(int fd)
{
        char    *line;
        char    buf[1];
        size_t  len;
        ssize_t rd;

        line = NULL;
        len = 0;
        while ((rd = read(fd, buf, 1)) > 0)
        {
                line = ft_realloc(line, len + 1, len + 2);
                if (!line)
                        return (NULL);
                line[len++] = buf[0];
                if (buf[0] == '\n')
                        break ;
        }
        if (rd <= 0 && len == 0)
                return (NULL);
        line[len] = '\0';
        return (line);
}
static void	write_expanded(int fd, char *line, t_ast *data)
{
	  t_args  arg;
        char    *tmp;
        char    *expanded;

	arg = (t_args){.argc = g_ctx->argc - 1, .argv = g_ctx->argv + 1,
                .exit_status = gles(g_ctx)};
        tmp = expand_tilde(line, data->env_list);
        if (!tmp)
                tmp = ft_strdup(line);
        expanded = parse_env(tmp, data->env_list, &arg);
        if (expanded)
        {
                ft_putendl_fd(expanded, fd);
                free(expanded);
        }
        else
                ft_putendl_fd(tmp, fd);
        free(tmp);
}

int	handle_line(int fd, char *line, t_hdinfo *info)
{
	if (!line)
	{
		ft_putstr_fd("bash: warning: here-document delimited by ",
			STDERR_FILENO);
		ft_putstr_fd("end-of-file (wanted `", STDERR_FILENO);
		ft_putstr_fd(info->delim, STDERR_FILENO);
		ft_putendl_fd("')", STDERR_FILENO);
		return (2);
	}
	if (ft_strcmp(line, info->delim) == 0)
		return (1);
	if (!info->quoted)
	{
		write_expanded(fd, line, info->data);
	}
	else
		ft_putendl_fd(line, fd);
	free(line);
	return (0);
}

int	run_heredoc_loop(int fd, t_hdinfo *info)
{
	char	*line;
	int		status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
			line = read_line_fd(STDIN_FILENO);
		if (!isatty(STDIN_FILENO) && line && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		status = handle_line(fd, line, info);
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

int	fork_heredoc(int fd, t_hdinfo *info)
{
	pid_t	pid;
	int		status;
	int		ret;

	pid = fork();
	if (pid == 0)
		exit(run_heredoc_loop(fd, info));
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
