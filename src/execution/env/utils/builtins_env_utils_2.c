/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:33:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/28 17:33:20 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h" /* your usual libft */


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

	node = NULL;
	node = (t_env *)malloc(sizeof(t_env));
	if (node == NULL)
		return (NULL);
	node->key = NULL;
	node->value = NULL;
	node->key = ft_strdup(key);
	if (val != NULL)
		node->value = ft_strdup(val);
	if (node->key == NULL || (val != NULL && node->value == NULL))
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
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

	node = NULL;
	node = find_env_node(lst, key);
	if (node == NULL)
		return (1);
	free(node->value);
	node->value = NULL;
	if (val != NULL)
		node->value = ft_strdup(val);
	if (val != NULL && node->value == NULL)
		return (1);
	return (0);
}
