/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 19:07:30 by ldurmish          #+#    #+#             */
/*   Updated: 2025/02/16 23:22:26 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*handle_special_utils(char *input, char *str, int *i, t_args *arg)
{
	if (input[*i] == '$')
	{
		if (input[*i + 1] == '?')
		{
			str = ft_itoa(arg->exit_status);
			*i += 2;
		}
		else if (input[*i + 1] == '@')
		{
			str = join_arguments(arg);
			*i += 2;
		}
		else if (input[*i + 1] == '*')
		{
			str = join_arguments(arg);
			*i += 2;
		}
		else if (input[*i + 1] == '0')
		{
			str = ft_strdup(arg->argv[0]);
			i += 2;
		}
	}
	return (str);
}

static char	*handle_special_var(char *input, int *i, t_args *arg)
{
	char	*str;

	str = NULL;
	if (input[*i] == '$')
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
	while (input[start + len] && (ft_isalnum(input[start + len])
			|| input[start + len] == '_'))
		len++;
	if (len == 0)
		return (NULL);
	name = ft_substr(input, start, len);
	*i = start + len;
	return (name);
}

static char	*handle_shlvl(t_env *env_list)
{
	char		*value;
	int			level;
	char		*new_value;

	value = get_env_value(env_list, "SHLVL");
	if (!value)
		level = 1;
	else
		level = ft_atoi(value) + 1;
	new_value = ft_itoa(level);
	return (new_value);
}

char	*env_expansion(char *input, int *i,
	t_env *env_list, t_args *arg)
{
	char		*value;
	char		*name;

	(*i)++;
	value = NULL;
	name = get_env_name(input, i, arg);
	if (!name)
		return (ft_strdup(""));
	if (!ft_strcmp(name, "SHLVL"))
	{
		value = handle_shlvl(env_list);
		free(name);
		return (value);
	}
	value = get_env_value(env_list, name);
	if (!value)
		value = ft_strdup("");
	else
		value = ft_strdup(value);
	free(name);
	return (value);
}
