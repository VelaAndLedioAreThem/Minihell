/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/04/10 21:21:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int	execute_command(t_ast *data, t_ast *tree, int fd_inp, int fd_out)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;
	char	**envp;
	int		i;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (fd_inp != STDIN_FILENO)
		{
			if (dup2(fd_inp, STDIN_FILENO) == -1)
			{
				perror("minishell: dup2 input redirection");
				exit(1);
			}
			close(fd_inp);
		}
		if (fd_out != STDOUT_FILENO)
		{
			if (dup2(fd_out, STDOUT_FILENO) == -1)
			{
				perror("minishell: dup2 output redirection");
				exit(1);
			}
			close(fd_out);
		}
		cmd_path = find_executable_path(data, tree->cmd->args[0]);
		if (!cmd_path)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(tree->cmd->args[0], STDERR_FILENO);
			ft_putendl_fd(": command not found", STDERR_FILENO);
			exit(127);
		}
		envp = env(&(data->env_list));
		execve(cmd_path, tree->cmd->args, envp);
		perror("minishell: execve");
		free_2darray(envp);
		exit(1);
	}
	if (fd_inp != STDIN_FILENO)
		close(fd_inp);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
	waitpid(pid, &status, 0);
	i = -1;
	while (++i < data->heredoc_count)
	{
		unlink(data->heredoc_files[i]);
		free(data->heredoc_files[i]);
	}
	free(data->heredoc_files);
	data->heredoc_files = NULL;
	data->heredoc_count = 0;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
