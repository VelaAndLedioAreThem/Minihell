/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_content.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 00:53:16 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/19 14:51:11 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

t_nest	check_paren_in_token(t_state *state, int end, char *input)
{
	t_nest			result;

	result = (t_nest){false, state->token, state->pos};
	while (state->pos < end && input[state->pos])
	{
		process_quotes(input[state->pos], &state->quote);
		if (!state->quote.in_double_quotes && !state->quote.in_single_quotes)
		{
			if (input[state->pos] == '(')
				state->paren_count++;
			else if (input[state->pos] == ')')
			{
				state->paren_count--;
				if (state->paren_count == 0)
				{
					result.success = true;
					result.position = state->pos;
					return (result);
				}
			}
		}
		state->pos++;
	}
	return (result);
}

t_nest	check_paren_in_multiple_token(t_state *state, char *input,
	t_token *curr)
{
	t_nest				result;

	result = (t_nest){false, curr, state->pos};
	while (input[state->pos] && state->paren_count > 0)
	{
		process_quotes(input[state->pos], &state->quote);
		if (!state->quote.in_double_quotes && !state->quote.in_single_quotes)
		{
			if (input[state->pos] == '(')
				state->paren_count++;
			else if (input[state->pos] == ')')
			{
				state->paren_count--;
				if (state->paren_count == 0)
					return ((t_nest){true, curr, state->pos});
			}
		}
		state->pos++;
	}
	return (result);
}

t_nest	validate_nested_paren(char *input, int start, int end,
	t_token *start_token)
{
	t_nest				result;
	t_state				state;
	t_token				*curr;

	result = (t_nest){false, start_token, start};
	state = (t_state){{false, false}, start_token, start + 1, 1};
	result = check_paren_in_token(&state, end, input);
	if (result.success)
		return (result);
	curr = state.token->next;
	while (curr && state.paren_count > 0)
	{
		state.pos = 0;
		input = curr->value;
		result = check_paren_in_multiple_token(&state, input, curr);
		if (result.success)
			return (result);
		curr = curr->next;
	}
	report_error(ERR_SYNTAX, "unbalanced parenthesis");
	return (result);
}

bool	handle_paren(char *input, t_open_paren *paren, int *end,
	t_token **curr)
{
	t_nest				result;

	result = validate_nested_paren(input, paren->i, *end, *curr);
	if (!result.success)
		return (false);
	if (result.token != *curr)
	{
		*curr = result.token;
		input = (*curr)->value;
		*end = ft_strlen(input);
		paren->i = result.position + 1;
	}
	else
		paren->i = result.position + 1;
	paren->has_commands = true;
	paren->expecting_commands = false;
	return (true);
}

bool	validate_paren_content(char *input, int start, int end,
	t_token *token)
{
	t_open_paren		paren;
	char				*curent_input;
	t_token				*curr;

	paren = (t_open_paren){false, true, start};
	curent_input = input;
	curr = token;
	while (paren.i < end)
	{
		if (skip_whitespaces(input, &paren.i, end))
			break ;
		if (curent_input[paren.i] == '(')
		{
			if (!handle_paren(input, &paren, &end, &curr))
				return (false);
			continue ;
		}
		if (op_paren(input, &paren, token) || pipe_paren(input, &paren, token)
			|| seq_paren(input, &paren, token))
			continue ;
		if (!validate_redirect_or_command(input, &paren, end, token))
			return (false);
	}
	return (validate_paren_content_utils(&paren, token));
}
