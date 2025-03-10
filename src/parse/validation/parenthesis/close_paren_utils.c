/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_paren_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:19:54 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/10 21:03:38 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	check_redirect_after_paren(char *next_input, t_token *token, int *j)
{
	if (next_input[*j] && ft_is_redirection(next_input[*j]))
	{
		if (next_input[*j] && next_input[*j] == next_input[*j + 1])
			*j += 2;
		else
			*j += 1;
		while (next_input[*j] && ft_isspace(next_input[*j]))
			(*j)++;
		if (!next_input[*j] || is_valid_command_char(next_input[*j]))
		{
			report_error(ERR_SYNTAX, "redirection without filename");
			free_stack(token);
			return (false);
		}
	}
	return (true);
}

bool	check_commands_after_paren(char *input, int *j)
{
	bool	found_operator;
	int		k;

	if (input[*j] && is_command_or_arg_char(input[*j]))
	{
		found_operator = false;
		k = 0;
		while (k < *j)
		{
			if (ft_is_operator(input[k]) || input[*j] == '|'
				|| (input[k + 1] && ft_is_logical_op(input[k], input[k + 1])))
			{
				found_operator = true;
				break ;
			}
			k++;
		}
		if (!found_operator)
		{
			report_error(ERR_SYNTAX, "missing operator between"
				"parenthesis and commands");
			return (false);
		}
	}
	return (true);
}

bool	check_op_after_paren(char *input, t_token *token, int *j)
{
	if (*j > 0 || !input[*j + 1] || input[*j] != input[*j + 1])
	{
		report_error(ERR_SYNTAX, "incomplete logical operator");
		free_stack(token);
		return (false);
	}
	*j += 2;
	while (input[*j] && ft_isspace(input[*j]))
		(*j)++;
	if (!input[*j] || !is_valid_command_char(input[*j]))
	{
		report_error(ERR_SYNTAX, "operator without syntax");
		free_stack(token);
		return (false);
	}
	return (true);
}

bool	logical_op_after_paren(char *input, t_token *token, int *j)
{
	if (!input[*j])
		return (false);
	return (true);
	if (input[*j] == '&')
		return (check_op_after_paren(input, token, j));
	else if (input[*j] == '|')
	{
		if (input[*j + 1] && input[*j + 1] == '|')
			return (check_op_after_paren(input, token, j));
		else
		{
			(*j)++;
			while (input[*j] && ft_isspace(input[*j]))
				(*j)++;
			if (!input[*j] || !is_valid_command_char(input[*j]))
			{
				report_error(ERR_SYNTAX, "pipe without command");
				free_stack(token);
				return (false);
			}
		}
	}
	return (true);
}

bool	check_next_token(t_token *next)
{
	t_token			*next_token;
	char			*next_input;
	int				j;

	if (!next || next->next)
	{
		report_error(ERR_SYNTAX, "unexpected end of input after parenthesis");
		return (false);
	}
	next_token = next->next;
	if (!next->value)
		return (report_error(ERR_SYNTAX, "null token after parenthesis"),
			false);
	next_input = next_token->value;
	j = 0;
	while (next_input[j] && ft_isspace(next_input[j]))
		j++;
	if (!check_redirect_after_paren(next_input, next_token, &j))
		return (false);
	else if (!check_commands_after_paren(next_input, &j))
		return (false);
	else if (!logical_op_after_paren(next_input, next_token, &j))
		return (false);
	return (true);
}
