/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_utils_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:59:21 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/07 19:57:44 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	validate_redirect_or_command(char *input, t_open_paren *paren,
	int end, t_token *token)
{
	if (validate_redirect_in_paren(input, paren, end, token))
		return (true);
	if (is_valid_command_char(input[paren->i]))
	{
		while (paren->i < end && is_valid_command_char(input[paren->i]))
			paren->i++;
		paren->has_commands = true;
		paren->expecting_commands = false;
	}
	else
	{
		report_error(ERR_SYNTAX, "invalid syntax near character");
		free_stack(token);
		return (false);
	}
	return (true);
}

bool	validate_seq_in_paren(char *input, t_open_paren *paren, t_token *token)
{
	if (input[paren->i] == ';')
	{
		if (!paren->has_commands)
		{
			report_error(ERR_SYNTAX, "sequence without preceding commands");
			free_stack(token);
			return (false);
		}
		paren->i++;
		paren->has_commands = false;
		paren->expecting_commands = true;
		return (true);
	}
	return (false);
}

bool	validate_pipe_in_paren(char *input, t_open_paren *paren, t_token *token)
{
	if (input[paren->i] == '|' && input[paren->i + 1] != '|')
	{
		if (!paren->has_commands)
		{
			report_error(ERR_SYNTAX, "pipe without preceding commands");
			free_stack(token);
			return (false);
		}
		paren->i++;
		paren->expecting_commands = true;
		paren->has_commands = false;
		return (true);
	}
	return (false);
}

bool	validate_nested_paren(char *input, int *i, int end,
	t_token *token)
{
	int			paren_count;
	t_token		*curr;
	int			start;
	t_quotes	quotes;

	paren_count = 1;
	start = *i + 1;
	quotes = (t_quotes){false, false};
	while (start < end && input[start])
	{
		process_quotes(input[start], &quotes);
		if (!quotes.in_double_quotes && !quotes.in_single_quotes)
		{
			if (input[start] == '(')
				paren_count++;
			else if (input[start] == ')')
			{
				paren_count--;
				if (paren_count == 0)
				{
					*i = start;
					return (true);
				}
			}
		}
		start++;
	}
	curr = token->next;
	while (curr && paren_count > 0)
	{
		start = 0;
		input = curr->value;
		while (input[start] && paren_count > 0)
		{
			process_quotes(input[start], &quotes);
			if (!quotes.in_single_quotes && !quotes.in_double_quotes)
			{
				if (input[start] == '(')
					paren_count++;
				else if (input[start] == ')')
				{
					paren_count--;
					if (paren_count == 0)
						return (true);
				}
			}
			start++;
		}
		curr = curr->next;
	}
	if (paren_count > 0)
	{
		report_error(ERR_SYNTAX, "unbalanced parentheses");
		free_stack(token);
		return (false);
	}
	return (true);
}

bool	validate_op_in_paren(char *input, t_open_paren *paren, t_token *token)
{
	if (ft_is_logical_op(input[paren->i], input[paren->i + 1]))
	{
		if (!paren->has_commands)
		{
			report_error(ERR_SYNTAX, "operator without preceding command");
			free_stack(token);
			return (false);
		}
		paren->i += 2;
		paren->expecting_commands = true;
		paren->has_commands = false;
		return (true);
	}
	return (false);
}
