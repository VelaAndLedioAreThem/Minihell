/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/04/10 21:21:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_pipe(t_ast *data, t_ast *tree)
{
	int		fd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		left_status;
	int		right_status;

	if (pipe(fd) == -1)
	{
		perror("minishell: pipe");
		return (data->exit_status = 1);
	}
	left_pid = fork();
	if (left_pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		exit(execute_tree(data, tree->left));
	}
	else if (left_pid < 0)
	{
		perror("minishell: fork");
		close(fd[0]);
		close(fd[1]);
		return (data->exit_status = 1);
	}
	right_pid = fork();
	if (right_pid == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		exit(execute_tree(data, tree->right));
	}
	else if (right_pid < 0)
	{
		perror("minishell: fork");
		close(fd[0]);
		close(fd[1]);
		kill(left_pid, SIGTERM);
		return (data->exit_status = 1);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);
	if (WIFEXITED(right_status))
		data->exit_status = WEXITSTATUS(right_status);
	else
		data->exit_status = 1;
	return (data->exit_status);
}
