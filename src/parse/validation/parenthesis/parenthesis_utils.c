/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 21:09:52 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/06 22:55:38 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	valid_before_open_paren(char c)
{
	return (ft_isspace(c) || c == '|' || c == '&' || c == '('
		|| c == ';' || c == '\0');
}

bool	is_valid_after_open_paren(char c)
{
	return (ft_isalnum(c) || c == '(' || c == '$');
}

bool	is_valid_close_paren(char c)
{
	return (ft_isspace(c) || c == '|' || c == '&' || c == ')' || c == ';'
		|| c == '\0');
}

static bool	skip_whitespaces(char *input, int *i, int end)
{
	while (*i < end && ft_isspace(input[*i]))
		(*i)++;
	return (*i >= end);
}

bool	validate_paren_content(char *input, int start, int end, t_token *token)
{
	t_open_paren	paren;

	paren = (t_open_paren){false, true, start};
	while (paren.i < end)
	{
		if (skip_whitespaces(input, &paren.i, end))
			break ;
		if (input[paren.i] == '(')
		{
			if (!validate_nested_paren(input, &paren.i, end, token))
				return (false);
			paren.has_commands = true;
			paren.expecting_commands = false;
			continue ;
		}
		if (paren.i + 1 >= (int)ft_strlen(input))
			return (false);
		if (validate_op_in_paren(input, &paren, token))
			continue ;
		if (validate_pipe_in_paren(input, &paren, token))
			continue ;
		if (validate_seq_in_paren(input, &paren, token))
			continue ;
		if (!validate_redirect_or_command(input, &paren, end, token))
			return (false);
	}
	return (validate_paren_content_utils(&paren, token));
}
