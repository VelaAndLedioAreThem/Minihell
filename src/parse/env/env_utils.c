/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 21:41:41 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/15 12:15:47 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	needs_shell_quoting(char *str)
{
	int		i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == '(' || str[i] == ')' || str[i] == ' '
			|| str[i] == '\t' || str[i] == '|' || str[i] == '&'
			|| str[i] == '<' || str[i] == '>')
			return (1);
		i++;
	}
	return (0);
}

static char	*add_protective_quotes(char *str)
{
	char	*result;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(len + 3);
	if (!result)
		return (NULL);
	result[0] = '"';
	ft_strcpy(result + 1, str);
	result[len + 1] = '"';
	result[len + 2] = '\0';
	return (result);
}

char	*safe_and_expand_var(char *value)
{
	char	*processed_value;
	char	*result;

	if (!value)
		return (ft_strdup(""));
	processed_value = remove_quotes_and_paren(value);
	if (!processed_value)
		processed_value = ft_strdup(value);
	if (needs_shell_quoting(processed_value))
	{
		result = add_protective_quotes(processed_value);
		free(processed_value);
		return (result);
	}
	return (processed_value);
}

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
