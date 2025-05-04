/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_validation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 17:59:54 by ldurmish          #+#    #+#             */
/*   Updated: 2025/05/04 17:55:05 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"
#include <stdbool.h>

void	process_quotes(char c, t_quotes *quote)
{
	if (c == '"' && !quote->in_single_quotes)
		quote->in_double_quotes = !quote->in_double_quotes;
	else if (c == '\'' && !quote->in_double_quotes)
		quote->in_single_quotes = !quote->in_single_quotes;
}

bool	parenthesis(t_token *token, char *input, t_paren *commands)
{
	int			i;

	i = 0;
	while (input[i] != '\0')
	{
		process_quotes(input[i], &commands->quote);
		if (!check_parenthesis(token, input, i, commands))
			return (false);
		i++;
	}
	return (true);
}

bool	parenthesis_utils(t_token *tokenize)
{
	char		*input;
	t_token		*stack;
	t_paren		commands;

	stack = tokenize;
	commands = (t_paren)
	{false, false, '\0', false, 0, false, (t_quotes){false, false}};
	initialize_stack(stack);
	while (stack && stack->value)
	{
		input = stack->value;
		if (!parenthesis(stack, input, &commands))
			return (false);
		stack = stack->next;
	}
	if (!is_empty(stack))
		return (report_error(ERR_UNEXPECTED_TOKEN, ")"),
			free_stack(stack), false);
	free_stack(stack);
	free(stack);
	return (true);
}

bool	check_count_paren(t_token *tokenize)
{
	int		paren_count;

	if (!tokenize)
		return (true);
	paren_count = count_parenthesis(tokenize);
	if (paren_count != 0)
	{
		if (paren_count > 0)
			report_error(ERR_UNEXPECTED_TOKEN, ")");
		else
			report_error(ERR_UNEXPECTED_TOKEN, "(");
		return (false);
	}
	if (!parenthesis_utils(tokenize))
		return (false);
	return (true);
}

bool	validation_parenthesis(t_token *tokenize)
{
	t_token		*current;
	t_token		*prev;
	t_token		*prev_non_ws;

	current = tokenize;
	prev = NULL;
	while (current != NULL)
	{
		if (current->type == TOKEN_PAREN_OPEN)
		{
			prev_non_ws = prev;
			if (prev_non_ws != NULL)
			{
				if (prev_non_ws->type == TOKEN_WORD)
					return (report_error(ERR_SYNTAX,
							"missing operator before '('"), false);
			}
		}
		if (current->type != TOKEN_WHITESPACE)
			prev = current;
		current = current->next;
	}
	if (!check_count_paren(tokenize))
		return (false);
	return (true);
}
