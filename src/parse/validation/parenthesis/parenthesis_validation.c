/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_validation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 17:59:54 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/05 01:31:37 by ldurmish         ###   ########.fr       */
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

bool	parenthesis(t_token *token, char *input, t_quotes *quote)
{
	int			i;

	i = 0;
	while (input[i] != '\0')
	{
		process_quotes(input[i], quote);
		if (!check_parenthesis(token, input, i, quote))
			return (false);
		i++;
	}
	return (true);
}

bool	parenthesis_utils(t_token *tokenize)
{
	char		*input;
	t_quotes	quote;
	t_token		*stack;

	stack = tokenize;
	quote = (t_quotes){false, false};
	initialize_stack(stack);
	while (stack && stack->value)
	{
		input = stack->value;
		if (!parenthesis(stack, input, &quote))
			return (false);
		stack = stack->next;
	}
	if (!is_empty(stack))
	{
		report_error(ERR_UNEXPECTED_TOKEN, "newline");
		free_stack(stack);
		return (false);
	}
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
