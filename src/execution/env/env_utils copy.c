/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils copy.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 12:34:56 by user              #+#    #+#             */
/*   Updated: 2025/04/10 21:18:25 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdlib.h>
#include <unistd.h>

void	incr_shell_lvl(t_env *data)
{
	char	*shlvl_str;
	int		shlvl;
	t_env	*tmp;

	shlvl_str = get_env_value(data, "SHLVL");
	shlvl = 0;
	if (shlvl_str)
		shlvl = ft_atoi(shlvl_str);
	shlvl++;
	tmp = data;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "SHLVL"))
		{
			free(tmp->value);
			tmp->value = ft_itoa(shlvl);
			return ;
		}
		tmp = tmp->next;
	}
	create_new_shlvl(data, shlvl);
}

void	set_env_var(t_ast *data, char *var_name, const char *var_value)
{
	t_env	*env;
	t_env	*new_env;

	env = data->env_list;
	while (env)
	{
		if (!ft_strcmp(env->key, var_name))
		{
			free(env->value);
			env->value = ft_strdup(var_value);
			return ;
		}
		env = env->next;
	}
	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return ;
	new_env->key = ft_strdup(var_name);
	new_env->value = ft_strdup(var_value);
	new_env->next = data->env_list;
	data->env_list = new_env;
}

t_env	*find_envir_variable(t_ast *data, char *var_name, int len)
{
	t_env	*current;

	current = data->env_list;
	while (current)
	{
		if (current->key && ft_strncmp(current->key, var_name, len) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

static char	*checker(t_ast *data, char *cmd, char ***paths_ptr)
{
	t_env	*path_env;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = find_envir_variable(data, "PATH", 4);
	if (!path_env || !path_env->value)
		return (NULL);
	*paths_ptr = ft_split(path_env->value, ':');
	if (!*paths_ptr)
		return (NULL);
	return (NULL);
}

char	*find_executable_path(t_ast *data, char *cmd)
{
	char	**paths;
	char	*full_path;
	char	*result;
	int		i;

	result = checker(data, cmd, &paths);
	if (result || !paths)
		return (result);
	i = -1;
	while (paths[++i])
	{
		full_path = ft_strjoin3(paths[i], "/", cmd);
		if (!full_path)
			continue ;
		if (access(full_path, X_OK) == 0)
		{
			free_2darray(paths);
			return (full_path);
		}
		free(full_path);
	}
	free_2darray(paths);
	return (NULL);
}
