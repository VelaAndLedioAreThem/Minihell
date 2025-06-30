/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_path_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:33:04 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/30 17:12:35 by vszpiech         ###   ########.fr       */
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

int	builtin_cd(t_ast *data, t_ast *tree, int fd)
{
	char	*path;
	char	*oldpwd;
	char	*expanded;
	int		count;

	(void)fd;
	path = cd_get_path(tree, &count);
	if (!path && count > 2)
		return (cd_too_many_args(data));
	expanded = NULL;
	if (cd_expand_tilde(&path, &expanded, data->env_list))
		return (cd_cleanup(data, NULL, expanded, 1));
	oldpwd = getcwd(NULL, 0);
	if (!path || !ft_strcmp(path, "~"))
		execute_home(data, path, oldpwd);
	if (path && cd_handle_dash(data, &path, oldpwd, expanded))
		return (1);
	if (execute_cd(data, path))
		return (cd_cleanup(data, oldpwd, expanded, 1));
	set_env_var(data, "OLDPWD", oldpwd);
	return (cd_cleanup(data, oldpwd, expanded, 0));
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
