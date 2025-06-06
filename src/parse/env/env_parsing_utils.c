/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 00:37:42 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/19 18:11:17 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*join_arguments(t_args *arg)
{
	int			i;
	char		*result;

	result = ft_strdup("");
	i = 0;
	while (i < arg->argc)
	{
		result = ft_strjoin(result, arg->argv[i]);
		if (i < arg->argc - 1)
			result = ft_strjoin(result, " ");
		i++;
	}
	return (result);
}

static int	quotes(char	*input, int i, t_args *parse)
{
	if (input[i] == '"')
	{
		if (!parse->single_quotes)
		{
			parse->double_quotes = !parse->double_quotes;
			if (parse->double_quotes)
				parse->last_quotes = '"';
			else if (parse->last_quotes == '"')
				parse->last_quotes = 0;
		}
		return (1);
	}
	if (input[i] == '\'')
	{
		if (!parse->double_quotes)
		{
			parse->single_quotes = !parse->single_quotes;
			if (parse->single_quotes)
				parse->last_quotes = '\'';
			else if (parse->last_quotes)
				parse->last_quotes = 0;
		}
		return (1);
	}
	return (0);
}

static char	*handle_env_part(t_args *parse, int *i, t_env *env_list)
{
	char		*temp;

	if (parse->single_quotes || parse->last_quotes == '\'')
	{
		temp = ft_substr(parse->input, parse->start, *i - parse->start);
		parse->result = ft_strjoin(parse->result, temp);
		free(temp);
		return (parse->result);
	}
	if (*i > parse->start)
	{
		temp = ft_substr(parse->input, parse->start, *i - parse->start);
		parse->result = ft_strjoin(parse->result, temp);
		free (temp);
	}
	temp = env_expansion(parse->input, i, env_list, parse);
	parse->result = ft_strjoin(parse->result, temp);
	free(temp);
	return (parse->result);
}

static void	handle_remaining(t_args *parse, int *i)
{
	char		*temp;

	if (*i > parse->start)
	{
		temp = ft_substr(parse->input, parse->start, *i - parse->start);
		parse->result = ft_strjoin(parse->result, temp);
		free(temp);
	}
}

char	*parse_env(char *input, t_env *env_list, t_args *arg)
{
	t_args		parse;
	int			i;

	i = 0;
    parse = (t_args){arg->argc, arg->argv, arg->exit_status, input, 0,
            ft_strdup(""), 0, 0, 0};
	while (input[i])
	{
		if (quotes(input, i, &parse))
		{
			i++;
			continue ;
		}
		if (input[i] == '$' && input[i + 1] && input[i + 1] != ' '
				&& input[i + 1] != '"' && input[i + 1] != '\'')
		{
			parse.result = handle_env_part(&parse, &i, env_list);
			parse.start = i;
		}
		else
			i++;
		if (i > ft_strlen(input))
			i = ft_strlen(input);
	}
	handle_remaining(&parse, &i);
	return (parse.result);
}
