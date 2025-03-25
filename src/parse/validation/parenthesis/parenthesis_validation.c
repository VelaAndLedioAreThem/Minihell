/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_validation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 17:59:54 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/19 01:18:42 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

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
	commands = (t_paren){false, false, '\0', false, 0, false, {false, false}};
	initialize_stack(stack);
	while (stack && stack->value)
	{
		input = stack->value;
		if (stack->single_quotes == 1)
			return (true);
		else if (stack->double_quotes == 1)
			return (true);
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
	if (!tokenize)
		return (true);
	if (!check_count_paren(tokenize))
		return (false);
	return (true);
}
