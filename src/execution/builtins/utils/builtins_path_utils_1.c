/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_path_utils_1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:33:04 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/30 17:12:15 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*cd_get_path(t_ast *tree, int *count)
{
	char	*path;

	*count = 0;
	while (tree->cmd->args && tree->cmd->args[*count])
		(*count)++;
	if (*count > 2)
		return (NULL);
	path = tree->cmd->args[1];
	if (path && !ft_strcmp(path, "--"))
	{
		path = tree->cmd->args[2];
		(*count)--;
	}
	if (*count > 2)
		return (NULL);
	return (path);
}

int	cd_expand_tilde(char **path, char **expanded, t_env *env)
{
	if (*path && (*path)[0] == '~')
	{
		*expanded = expand_tilde(*path, env);
		if (!*expanded && (*path)[1] && (*path)[1] != '/' && (*path)[1] != '+'
			&& (*path)[1] != '-')
			return (1);
		if (*expanded)
			*path = *expanded;
	}
	return (0);
}

int	cd_handle_dash(t_ast *data, char **path, char *oldpwd,
		char *expanded)
{
	if (!ft_strcmp(*path, "-"))
	{
		execute_oldpwd(data, *path, oldpwd);
		*path = get_env_value(data->env_list, "OLDPWD");
		if (!*path)
		{
			free(oldpwd);
			if (expanded)
				free(expanded);
			data->exit_status = 1;
			return (1);
		}
	}
	return (0);
}

int	cd_cleanup(t_ast *data, char *oldpwd, char *expanded, int err)
{
	free(oldpwd);
	if (expanded)
		free(expanded);
	data->exit_status = err;
	return (1);
}
