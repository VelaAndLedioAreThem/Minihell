/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vela <vela@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 19:07:30 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/13 00:28:17 by ldurmish         ###   ########.fr       */
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

static char	*get_env_name(char *input, int *i)
{
	int		start;
	int		len;
	char	*name;

	start = *i;
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

static char	*handle_variable_parsing(char *input, int *i, t_args *arg,
				int *is_special)
{
	char	*result;

	*is_special = 0;
	result = handle_special_utils(input, NULL, i, arg);
	if (result)
	{
		*is_special = 1;
		return (result);
	}
	(*i)--;
	return (get_env_name(input, i));
}

char	*env_expansion(char *input, int *i, t_env *env_list, t_args *arg)
{
	char	*name;
	char	*val;
	char	*value;
	int		is_special;

	(*i)++;
	name = handle_variable_parsing(input, i, arg, &is_special);
	if (is_special)
		return (name);
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
		value = ft_strdup("");
	else
		value = ft_strdup(value);
	free(name);
	return (value);
}
