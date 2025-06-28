/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_path_utils_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vela <vela@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:58:36 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/29 00:46:22 by vela             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*get_env_node(t_env *env_list, const char *name)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	update_env_var(t_ast *data, const char *key, const char *value)
{
	t_env	*env;
	t_env	*new_env;

	env = get_env_node(data->env_list, key);
	if (env)
	{
		free(env->value);
		env->value = ft_strdup(value);
	}
	else
	{
		new_env = malloc(sizeof(t_env));
		new_env->key = ft_strdup(key);
		new_env->value = ft_strdup(value);
		new_env->next = data->env_list;
		data->env_list = new_env;
	}
}

int	update_directory(t_ast *data, char *path, char *old_pwd)
{
	char	*new_pwd;

	if (chdir(path) != 0)
	{
		ft_putstr_fd("bash: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		free(old_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (handle_pwd_errors(old_pwd, 1));
	update_env_var(data, "OLDPWD", old_pwd);
	update_env_var(data, "PWD", new_pwd);
	free(old_pwd);
	free(new_pwd);
	return (0);
}

int	execute_cd(t_ast *data, char *path)
{
	char	*old_pwd;
	t_env	*home;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (handle_pwd_errors(NULL, 1));
	if (!path)
	{
		home = get_env_node(data->env_list, "HOME");
		if (!home || !home->value)
		{
			ft_putendl_fd("bash: cd: HOME not set", STDERR_FILENO);
			free(old_pwd);
			return (1);
		}
		path = home->value;
	}
	return (update_directory(data, path, old_pwd));
}

int	execute_home(t_ast *data, char *path, char *oldpwd)
{
	(void)oldpwd;
	path = get_env_value(data->env_list, "HOME");
	if (!path)
	{
		ft_putendl_fd("bash: cd: HOME not set", STDERR_FILENO);
		return (data->exit_status = 1, 1);
	}
	return (0);
}
