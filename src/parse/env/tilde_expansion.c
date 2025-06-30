/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 16:59:32 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/30 17:07:04 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static const char	*tilde_select_prefix(const char **rest, t_env *env)
{
	if (**rest == '\0' || **rest == '/')
		return (get_env_value(env, "HOME"));
	if (**rest == '+')
	{
		(*rest)++;
		return (get_env_value(env, "PWD"));
	}
	if (**rest == '-')
	{
		(*rest)++;
		return (get_env_value(env, "OLDPWD"));
	}
	return (NULL);
}

char	*expand_tilde(const char *path, t_env *env)
{
	const char	*rest;
	const char	*prefix;

	if (!path || path[0] != '~')
	{
		if (path)
			return (ft_strdup(path));
		else
		{
			return (NULL);
		}
	}
	rest = path + 1;
	prefix = tilde_select_prefix(&rest, env);
	if (!prefix)
	{
		if (*rest)
			ft_putendl_fd("minishell: no such user", STDERR_FILENO);
		return (NULL);
	}
	return (ft_strjoin(prefix, rest));
}

void	expand_tilde_tokens(t_token *tokens, t_env *env)
{
	t_token	*curr;
	char	*expanded;

	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_WORD && !curr->quotes.in_single_quotes
			&& curr->value && curr->value[0] == '~')
		{
			expanded = expand_tilde(curr->value, env);
			if (expanded)
			{
				free(curr->value);
				curr->value = expanded;
			}
		}
		curr = curr->next;
	}
}

t_env	*create_copy_env_node(t_env *original)
{
	t_env	*env_node;

	env_node = malloc(sizeof(t_env));
	if (!env_node)
		return (NULL);
	env_node->key = NULL;
	env_node->next = NULL;
	env_node->value = NULL;
	if (original->key)
	{
		env_node->key = ft_strdup(original->key);
		if (!env_node->key)
		{
			free(env_node);
			return (NULL);
		}
	}
	if (original->value)
	{
		env_node->value = ft_strdup(original->value);
		if (!env_node->value)
			return (free(env_node->key), free(env_node), NULL);
	}
	return (env_node);
}

t_env	*deep_copy_env_list(t_env *env_list)
{
	t_env	*new_head;
	t_env	*new_curr;
	t_env	*new_node;
	t_env	*original;

	new_head = NULL;
	new_curr = NULL;
	original = env_list;
	while (original)
	{
		new_node = create_copy_env_node(original);
		if (!new_node)
		{
			free_env_list(new_head);
			return (NULL);
		}
		if (!new_head)
			new_head = new_node;
		else
			new_curr->next = new_node;
		new_curr = new_node;
		original = original->next;
	}
	return (new_head);
}
