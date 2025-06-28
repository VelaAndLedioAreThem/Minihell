/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_word_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/28 15:34:52 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Find command path and handle errors
char	*get_command_path(t_ast *data, char *cmd)
{
	char	*cmd_path;

	cmd_path = find_executable_path(data, cmd);
	if (!cmd_path)
	{
		ft_putstr_fd("bash: ", STDERR_FILENO);
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
	print_error(tree->cmd->args[0], strerror(errno));
	free(cmd_path);
	free_2darray(envp);
	exit(126);
}

// Handle parent process waiting for child
int	parent_process(pid_t pid, t_ast *data)
{
	int	status;

	g_ctx->child_pid = pid;
	waitpid(pid, &status, 0);
	g_ctx->child_pid = 0;
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->exit_status = 128 + WTERMSIG(status);
	else
		data->exit_status = 1;
	return (data->exit_status);
}
