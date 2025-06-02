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
