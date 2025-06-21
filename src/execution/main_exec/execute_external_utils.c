/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/04/12 15:12:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	handle_redirection(int fd_inp, int fd_out)
{
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
}

void	display_command_not_found(char *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
}

void	execute_child_process(t_ast *data, t_ast *tree)
{
	char	*cmd_path;
	char	**envp;

	cmd_path = find_executable_path(data, tree->cmd->args[0]);
	if (!cmd_path)
	{
		display_command_not_found(tree->cmd->args[0]);
		exit(127);
	}
	envp = env(&(data->env_list));
	execve(cmd_path, tree->cmd->args, envp);
	perror("minishell: execve");
	free(cmd_path);
	free_2darray(envp);
	exit(1);
}

void	child_process_handler(t_ast *data, t_ast *tree, int fd_inp, int fd_out)
{
	setup_child_signals();
	handle_redirection(fd_inp, fd_out);
	execute_child_process(data, tree);
}
