/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/03/25 13:17:20 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

bool	is_valid_identifier(const char *name)
{
	if (!name || !*name || ft_isdigit(*name))
		return (false);
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (false);
		name++;
	}
	return (true);
}

t_env	*get_env_node(t_env *env_list, const char *name)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (strcmp(current->key, name) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

static void	update_env_var(t_ast *data, const char *key, const char *value)
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

int	execute_echo(char *args[], int fd_out)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 1;
	if (args[1] && !ft_strcmp(args[1], "-n"))
	{
		n_flag = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], fd_out);
		if (args[i + 1])
			ft_putstr_fd(" ", fd_out);
		i++;
	}
	if (n_flag)
		ft_putendl_fd("", fd_out);
	return (0);
}

static int	handle_pwd_errors(char *old_pwd, int error_code)
{
	free(old_pwd);
	ft_putendl_fd(" error retrieving current directory", STDERR_FILENO);
	return (error_code);
}

static int	update_directory(t_ast *data, char *path, char *old_pwd)
{
	char	*new_pwd;

	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
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
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
			free(old_pwd);
			return (1);
		}
		path = home->value;
	}
	return (update_directory(data, path, old_pwd));
}

int	create_new_env(t_ast *d, char *n, char *eq)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (free(n), 1);
	env->key = n;
	if (eq)
		env->value = ft_strdup(eq + 1);
	else
		env->value = ft_strdup("");
	env->next = d->env_list;
	d->env_list = env;
	return (0);
}

int	execute_exit(t_ast *data, t_ast *tree)
{
	int	status;

	status = 0;
	if (tree->cmd->args[1])
		status = ft_atoi(tree->cmd->args[1]);
	free_data(data);
	exit (status);
	return (0);
}
