/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 21:09:52 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/05 02:07:50 by ldurmish         ###   ########.fr       */
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

int	find_matching_paren(t_token *token, char *input, int start_pos)
{
	int			paren_count;
	int			j;
	t_quotes	temp_quote;
	t_token		*curr;
	int			total_pos;

	paren_count = 1;
	curr = token;
	while (curr && start_pos >= ft_strlen(curr->value))
	{
		start_pos -= ft_strlen(curr->value);
		curr = curr->next;
	}
	j = start_pos;
	temp_quote = (t_quotes){false, false};
	total_pos = 0;
	while (curr)
	{
		input = curr->value;
		while (input[j] != '\0')
		{
			process_quotes(input[j], &temp_quote);
			if (!temp_quote.in_double_quotes && !temp_quote.in_single_quotes)
			{
				if (input[j] == '(')
					paren_count++;
				else if (input[j] == ')')
					paren_count--;
			}
			if (paren_count == 0)
				return (total_pos + j);
			j++;
		}
		total_pos += ft_strlen(curr->value);
		j = 0;
		curr = curr->next;
	}
	return (-1);
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
