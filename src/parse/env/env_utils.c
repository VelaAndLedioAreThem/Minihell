/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 21:41:41 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/11 08:50:46 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_env	*create_env_value(t_env *new_node, char *raw_value)
{
	char		*processed_value;

	processed_value = remove_quotes_and_paren(raw_value);
	if (!processed_value)
	{
		new_node->value = ft_strdup(raw_value);
		if (!new_node->value)
		{
			free(new_node->key);
			free(new_node);
			return (NULL);
		}
	}
	else
	{
		new_node->value = processed_value;
		if (!new_node->value)
		{
			free(new_node->key);
			free(new_node);
			return (NULL);
		}
	}
	new_node->next = NULL;
	return (new_node);
}

t_env	*create_env_node(char *input)
{
	t_env	*new_node;
	char	*equal_sign;
	char	*raw_value;

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
	if (!new_node->key)
	{
		free(new_node);
		return (NULL);
	}
	raw_value = equal_sign + 1;
	return (create_env_value(new_node, raw_value));
}
