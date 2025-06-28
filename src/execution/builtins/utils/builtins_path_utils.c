/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_path_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:33:04 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/28 14:26:09 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pwd_errors(char *old_pwd, int error_code)
{
	ft_putstr_fd("minishell: cd: error retrieving current directory: getcwd: ", STDERR_FILENO);
    ft_putendl_fd(strerror(errno), STDERR_FILENO);
    free(old_pwd);
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

int	builtin_cd(t_ast *data, t_ast *tree, int fd)
{
	int		count;
	char	*path;
	char	*oldpwd;

	(void)fd;
	count = 0;
	while (tree->cmd->args && tree->cmd->args[count])
		count++;
	if (count > 2)
		return (cd_too_many_args(data));
	path = tree->cmd->args[1];
	oldpwd = getcwd(NULL, 0);
	if (!path || !ft_strcmp(path, "~"))
		execute_home(data, path, oldpwd);
	else if (!ft_strcmp(path, "-"))
		execute_oldpwd(data, path, oldpwd);
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
	char	*pwd;

	pwd = get_env_value(data->env_list, "PWD");
	if (pwd)
	{
		ft_putendl_fd(pwd, fd_out);
		return (0);
	}
	ft_putendl_fd("minishell: pwd: PWD not set", STDERR_FILENO);
	return (1);
}
