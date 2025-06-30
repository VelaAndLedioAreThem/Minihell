/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 00:37:42 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/30 15:25:44 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*join_arguments(t_args *arg)
{
	int			i;
	char		*result;
	char		*temp;

	result = ft_strdup("");
	i = 0;
	while (i < arg->argc)
	{
		temp = ft_strjoin(result, arg->argv[i]);
		free(result);
		result = temp;
		if (!result)
			return (NULL);
		if (i < arg->argc - 1)
		{
			temp = ft_strjoin(result, " ");
			free(result);
			result = temp;
			if (!result)
				return (NULL);
		}
		i++;
	}
	return (result);
}

int	quotes(char	*input, int i, t_args *parse)
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

char	*handle_env_part(t_args *parse, int *i, t_env *env_list)
{
	if (parse->single_quotes)
	{
		parse->temp = ft_substr(parse->input, parse->start, *i - parse->start + 1);
		parse->old_result = parse->result;
		parse->result = ft_strjoin(parse->result, parse->temp);
		free(parse->old_result);
		free(parse->temp);
		(*i)++;
		return (parse->result);
	}
	if (*i > parse->start)
	{
		parse->temp = ft_substr(parse->input, parse->start, *i - parse->start);
		parse->old_result = parse->result;
		parse->result = ft_strjoin(parse->result, parse->temp);
		free(parse->old_result);
		free (parse->temp);
	}
	parse->temp = env_expansion(parse->input, i, env_list, parse);
	parse->old_result = parse->result;
	parse->result = ft_strjoin(parse->result, parse->temp);
	free(parse->old_result);
	free(parse->temp);
	return (parse->result);
}

static void	handle_remaining(t_args *parse, int *i)
{
	char		*temp;
	char		*new_result;

	if (*i > parse->start)
	{
		temp = ft_substr(parse->input, parse->start, *i - parse->start);
		if (!temp)
		{
			free(parse->result);
			parse->result = NULL;
			return ;
		}
		new_result = ft_strjoin(parse->result, temp);
		free(temp);
		if (!new_result)
		{
			free(parse->result);
			parse->result = NULL;
			return ;
		}
		free(parse->result);
		parse->result = new_result;
	}
}

char	*parse_env(char *input, t_env *env_list, t_args *arg)
{
	t_args		parse;

	parse = (t_args){arg->argc, arg->argv, arg->exit_status, input, 0,
		ft_strdup(""), 0, 0, 0, 0, NULL, NULL};
	if (!parse.result)
		return (NULL);
	while (input[parse.i] && parse.i < ft_strlen(input))
		process_env_var(&parse, env_list, input);
	handle_remaining(&parse, &parse.i);
	return (parse.result);
}
