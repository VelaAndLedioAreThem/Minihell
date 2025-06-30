/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 19:07:30 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/30 16:52:23 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*handle_special_var(char *input, int *i, t_args *arg)
{
	char	*str;

	str = NULL;
	str = handle_special_utils(input, str, i, arg);
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

char	*expand_value(char *in, int *i, t_env *env, t_args *arg)
{
	char	*name;
	char	*val;

	if (in[*i] == '?' || in[*i] == '@' || in[*i] == '*'
		|| in[*i] == '0')
		return (handle_special_var(in, i, arg));
	if (!ft_isalpha(in[*i]) && in[*i] != '_')
		return ((*i)--, ft_strdup("$"));
	name = get_env_name(in, i);
	if (!name)
		return (ft_strdup(""));
	if (!ft_strcmp(name, "SHLVL"))
	{
		val = handle_shlvl(env);
		return (free(name), val);
	}
	val = get_env_value(env, name);
	if (!val)
		return (free(name), ft_strdup(""));
	val = process_env_value(val, arg);
	return (free(name), val);
}

char	*env_expansion(char *input, int *i, t_env *env_list, t_args *arg)
{
	(*i)++;
	if (!input[*i])
		return ((*i)--, ft_strdup("$"));
	return (expand_value(input, i, env_list, arg));
}
