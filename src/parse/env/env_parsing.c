/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vela <vela@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 19:07:30 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/15 02:02:53 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*handle_special_utils(char *input, char *str, int *i, t_args *arg)
{
	if (input[*i] == '?')
	{
		str = ft_itoa(arg->exit_status);
		(*i)++;
	}
	else if (input[*i] == '@' || input[*i] == '*')
	{
		str = join_arguments(arg);
		(*i)++;
	}
	else if (input[*i] == '0')
	{
		str = ft_strdup(arg->argv[0]);
		(*i)++;
	}
	return (str);
}

static char	*handle_special_var(char *input, int *i, t_args *arg)
{
	char	*str;

	str = NULL;
	str = handle_special_utils(input, str, i, arg);
	return (str);
}

static char	*get_env_name(char *input, int *i, t_args *arg)
{
	int		start;
	int		len;
	char	*name;
	char	*special;

	start = *i;
	special = handle_special_var(input, i, arg);
	if (special)
		return (special);
	len = 0;
	while (input[start + len] && (ft_isalnum(input[start + len]) || input[start
				+ len] == '_'))
		len++;
	if (len == 0)
		return (NULL);
	name = ft_substr(input, start, len);
	*i = start + len;
	return (name);
}

static char	*handle_shlvl(t_env *env_list)
{
	char	*value;
	int		level;
	char	*new_value;

	value = get_env_value(env_list, "SHLVL");
	if (!value)
		level = 1;
	else
		level = ft_atoi(value) + 1;
	new_value = ft_itoa(level);
	return (new_value);
}

char	*safe_expand_env_var(char *value)
{
	char	*processed_value;
	char	*result;
	int		needs_quotes;
	int		i;

	if (!value)
		return (ft_strdup(""));
	
	processed_value = remove_quotes_and_paren(value);
	if (!processed_value)
		processed_value = ft_strdup(value);
	
	/* Check if the content needs to be quoted for safe tokenization */
	needs_quotes = 0;
	i = 0;
	while (processed_value[i])
	{
		if (processed_value[i] == '(' || processed_value[i] == ')' ||
		    processed_value[i] == ' ' || processed_value[i] == '\t' ||
		    processed_value[i] == '|' || processed_value[i] == '&' ||
		    processed_value[i] == '<' || processed_value[i] == '>')
		{
			needs_quotes = 1;
			break;
		}
		i++;
	}
	
	if (needs_quotes)
	{
		result = malloc(ft_strlen(processed_value) + 3); /* +2 for quotes, +1 for null */
		if (!result)
		{
			free(processed_value);
			return (NULL);
		}
		sprintf(result, "\"%s\"", processed_value);
		free(processed_value);
		return (result);
	}
	
	return (processed_value);
}

/* Updated env_expansion function in env_parsing.c */
char	*env_expansion(char *input, int *i, t_env *env_list, t_args *arg)
{
	char	*val;
	char	*name;
	char	*value;

	(*i)++;
	name = get_env_name(input, i, arg);
	if (!name)
		return (ft_strdup(""));
	if (!ft_strcmp(name, "SHLVL"))
	{
		val = handle_shlvl(env_list);
		free(name);
		return (val);
	}
	value = get_env_value(env_list, name);
	if (!value)
		return (free(name), ft_strdup(""));
	
	/* Use safe expansion that preserves tokenization safety */
	val = safe_expand_env_var(value);
	free(name);
	return (val);
}
