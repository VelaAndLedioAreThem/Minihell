/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/00/00 00:00:00 by user              #+#    #+#             */
/*   Updated: 2023/00/00 00:00:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pwd_errors(char *old_pwd, int error_code)
{
	free(old_pwd);
	ft_putendl_fd(" error retrieving current directory", STDERR_FILENO);
	return (error_code);
}

int	execute_oldpwd(t_ast *data, char *path, char *oldpwd)
{
	path = get_env_value(data->env_list, "OLDPWD");
	if (!path)
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR_FILENO);
		free(oldpwd);
		return (data->exit_status = 1, 1);
	}
	ft_putendl_fd(path, STDOUT_FILENO);
	return (0);
}

int	builtin_cd(t_ast *data, t_ast *tree, int fd_out)
{
	char	*path;
	char	*oldpwd;

	(void)fd_out;
	path = tree->cmd->args[1];
	oldpwd = getcwd(NULL, 0);
	if (!path || !ft_strcmp(path, "~"))
	{
		execute_home(data, path, oldpwd);
	}
	else if (!ft_strcmp(path, "-"))
	{
		execute_oldpwd(data, path, oldpwd);
	}
	if (execute_cd(data, path))
	{
		free(oldpwd);
		return (data->exit_status = 1, 1);
	}
	set_env_var(data, "OLDPWD", oldpwd);
	free(oldpwd);
	data->exit_status = 0;
	return (1);
}

int	builtin_pwd(t_ast *data, t_ast *tree, int fd_out)
{
	(void)tree;
	if (execute_pwd(data, fd_out))
		return (data->exit_status = 1, 1);
	data->exit_status = 0;
	return (1);
}

int	execute_pwd(t_ast *data, int fd_out)
{
	char	*cwd;

	(void)data;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putendl_fd("minishell: pwd: error retrieving current directory",
			STDERR_FILENO);
		return (1);
	}
	ft_putendl_fd(cwd, fd_out);
	free(cwd);
	return (0);
}
