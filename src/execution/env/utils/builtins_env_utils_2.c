/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: you <you@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:05:00 by you               #+#    #+#             */
/*   Updated: 2025/06/05 12:05:00 by you              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h" /* your usual libft */

/* --- basic linked-list helpers ------------------------------------------ */

t_env	*find_env_node(t_env *lst, const char *key)
{
	while (lst)
	{
		if (!ft_strcmp(lst->key, key))
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

t_env	*new_node(const char *key, const char *val)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = val ? ft_strdup(val) : NULL;
	if (!node->key || (val && !node->value))
	{
		free(node->key);
		free(node->value);
		return (free(node), NULL);
	}
	node->next = NULL;
	return (node);
}


int	add_env_node(t_env **lst, t_env *new_node)
{
	t_env	*cur;

	if (!new_node)
		return (1);
	if (!*lst)
		return (*lst = new_node, 0);
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = new_node;
	return (0);
}

int	update_env_value(t_env *lst, const char *key, const char *val)
{
	t_env	*node;

	node = find_env_node(lst, key);
	if (!node)
		return (1);
	free(node->value);
	node->value = val ? ft_strdup(val) : NULL;
	return (node->value || !val ? 0 : 1);
}
