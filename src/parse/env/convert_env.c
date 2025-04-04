/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 20:29:25 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/19 18:11:32 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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

	if (!env_list)
		return (NULL);
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

char	*get_env_value(t_env *env_list, char *name)
{
	t_env		*current;

	current = env_list;
	while (current)
	{
		if (!ft_strcmp(current->key, name))
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

t_env	*create_env_node(char *input)
{
	t_env		*new_node;
	char		*equal_sign;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	equal_sign = ft_strchr(input, '=');
	if (!equal_sign)
	{
		free(new_node);
		return (NULL);
	}
	new_node->key = ft_substr(input, 0, equal_sign - input);
	new_node->value = ft_strdup(equal_sign + 1);
	new_node->next = NULL;
	return (new_node);
}

t_env	*init_env_list(char **envp)
{
	t_env		*head;
	t_env		*current;
	int			i;
	t_env		*new_node;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = create_env_node(envp[i]);
		if (!new_node)
		{
			free_env_list(new_node);
			return (NULL);
		}
		if (!head)
			head = new_node;
		else
			current->next = new_node;
		current = new_node;
		i++;
	}
	return (head);
}
