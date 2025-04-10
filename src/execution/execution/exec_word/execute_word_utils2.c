/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_word_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/04/10 21:35:35 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// Setup child process signals
void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

// Find command path and handle errors
char	*get_command_path(t_ast *data, char *cmd)
{
	char	*cmd_path;

	cmd_path = find_executable_path(data, cmd);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		exit(127);
	}
	return (cmd_path);
}

// Execute command in child process
void	execute_command(t_ast *data, t_ast *tree, int fd_in, int fd_out)
{
	char	*cmd_path;
	char	**envp;

	setup_child_redirections(fd_in, fd_out);
	setup_child_signals();
	cmd_path = get_command_path(data, tree->cmd->args[0]);
	envp = env(&(data->env_list));
	execve(cmd_path, tree->cmd->args, envp);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	free(cmd_path);
	free_2darray(envp);
	exit(126);
}

// Handle parent process waiting for child
int	parent_process(pid_t pid, t_ast *data)
{
	int	status;

	g_child_pid = pid;
	waitpid(pid, &status, 0);
	g_child_pid = 0;
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->exit_status = 128 + WTERMSIG(status);
	else
		data->exit_status = 1;
	return (data->exit_status);
}
