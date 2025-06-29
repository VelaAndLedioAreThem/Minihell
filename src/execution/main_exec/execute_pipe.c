/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/04/10 21:44:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	fork_left_process(t_ast *data, t_ast *tree, int fd[2])
{
	pid_t	left_pid;

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
		perror("bash: fork");
		return (-1);
	}
	return (left_pid);
}

pid_t	fork_right_process(t_ast *data, t_ast *tree, int fd[2])
{
	pid_t	right_pid;

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
		perror("bash: fork");
		return (-1);
	}
	return (right_pid);
}

int	handle_fork_error(t_ast *data, int fd[2], pid_t left_pid)
{
	close_pipe(fd);
	if (left_pid > 0)
		kill(left_pid, SIGTERM);
	return (data->exit_status = 1);
}

int	wait_for_processes(t_ast *data, pid_t left_pid, pid_t right_pid)
{
	int	left_status;
	int	right_status;

	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);
	if (WIFEXITED(right_status))
		data->exit_status = WEXITSTATUS(right_status);
	else
		data->exit_status = 1;
	return (data->exit_status);
}

int	execute_pipe(t_ast *data, t_ast *tree)
{
        int             fd[2];
        pid_t   left_pid;
        pid_t   right_pid;

        if (setup_pipe(fd) != 0)
                return (data->exit_status = 1);
        left_pid = fork_left_process(data, tree, fd);
        if (left_pid < 0)
                return (handle_fork_error(data, fd, 0));
        close(fd[1]);
        fd[1] = -1;
        right_pid = fork_right_process(data, tree, fd);
        if (right_pid < 0)
                return (handle_fork_error(data, fd, left_pid));
        close(fd[0]);
        fd[0] = -1;
        close_pipe(fd);
        return (wait_for_processes(data, left_pid, right_pid));
}

