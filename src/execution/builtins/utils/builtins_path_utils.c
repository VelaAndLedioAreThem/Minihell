/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_path_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:33:04 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/30 12:43:03 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pwd_errors(char *old_pwd, int error_code)
{
	ft_putstr_fd("bash: cd: error retrieving current directory: getcwd: ",
		STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	free(old_pwd);
	return (error_code);
}

int	execute_oldpwd(t_ast *data, char *path, char *oldpwd)
{
	(void)oldpwd;
	path = get_env_value(data->env_list, "OLDPWD");
	if (!path)
	{
		ft_putendl_fd("bash: cd: OLDPWD not set", STDERR_FILENO);
		return (data->exit_status = 1, 1);
	}
	ft_putendl_fd(path, STDOUT_FILENO);
	return (0);
}

int	builtin_cd(t_ast *data, t_ast *tree, int fd)
{
	int		count;
	char	*path;
	char	*oldpwd;
	char	*expanded;

	(void)fd;
	count = 0;
	while (tree->cmd->args && tree->cmd->args[count])
		count++;
	if (count > 2)
		return (cd_too_many_args(data));
	path = tree->cmd->args[1];
	expanded = NULL;
	if (path && !ft_strcmp(path, "--"))
	{
		path = tree->cmd->args[2];
		count--;
	}
	if (count > 2)
		return (cd_too_many_args(data));
	if (path && path[0] == '~')
	{
		expanded = expand_tilde(path, data->env_list);
		if (!expanded && path[1] && path[1] != '/' && path[1] != '+'
			&& path[1] != '-')
			return (data->exit_status = 1, 1);
		if (expanded)
			path = expanded;
	}
	oldpwd = getcwd(NULL, 0);
	if (!path || !ft_strcmp(path, "~"))
		execute_home(data, path, oldpwd);
	else if (!ft_strcmp(path, "-"))
	{
		execute_oldpwd(data, path, oldpwd);
		path = get_env_value(data->env_list, "OLDPWD");
		if (!path)
		{
			free(oldpwd);
			if (expanded)
				free(expanded);
			return (data->exit_status = 1, 1);
		}
	}
	if (!path || !ft_strcmp(path, "~"))
		execute_home(data, path, oldpwd);
	else if (!ft_strcmp(path, "-"))
		execute_oldpwd(data, path, oldpwd);
	if (execute_cd(data, path))
	{
		free(oldpwd);
		if (expanded)
			free(expanded);
		return (data->exit_status = 1, 1);
	}
	set_env_var(data, "OLDPWD", oldpwd);
	free(oldpwd);
	if (expanded)
		free(expanded);
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
	char	*pwd;

	pwd = get_env_value(data->env_list, "PWD");
	if (pwd)
	{
		ft_putendl_fd(pwd, fd_out);
		return (0);
	}
	ft_putendl_fd("bash: pwd: PWD not set", STDERR_FILENO);
	return (1);
}
