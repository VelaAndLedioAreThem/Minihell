/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_pipes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:22:21 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/12 21:45:22 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	is_only_whitespaces(char *str)
{
	int			i;

	i = 0;
	if (!str)
		return (false);
	while (str[i] != '\0')
	{
		if (!ft_isspace(str[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	check_pipe_followed_by_logical(t_token *curr)
{
	t_token		*next;

	next = curr->next;
	while (next && next->type == TOKEN_WORD && is_only_whitespaces(next->value))
		next = next->next;
	if (next && (next->type == TOKEN_AND || next->type == TOKEN_OR))
	{
		if (next->type == TOKEN_AND && ft_strcmp(next->value, "&&") == 0)
			report_error(ERR_SYNTAX, "invalid operator sequence '|&&'");
		else if (next->type == TOKEN_OR && ft_strcmp(next->value, "||") == 0)
			report_error(ERR_SYNTAX, "invalid opeartor sequence '|||'");
		else
			report_error(ERR_SYNTAX, "invalid operator sequence");
		return (false);
	}
	return (true);
}

bool	validate_pipe_position(t_token *prev, t_token *curr)
{
	t_token		*next;

	if (prev == NULL
		|| (prev->type == TOKEN_WORD && is_only_whitespaces(prev->value)))
	{
		report_error(ERR_UNEXPECTED_TOKEN, "'|'");
		return (false);
	}
	if (curr->next == NULL)
	{
		report_error(ERR_UNEXPECTED_TOKEN, "'|'");
		return (false);
	}
	if (!check_pipe_followed_by_logical(curr))
		return (false);
	next = curr->next;
	while (next && next->type == TOKEN_WORD && is_only_whitespaces(next->value))
		next = next->next;
	if (next && next->type == TOKEN_PIPE)
		return (report_error(ERR_UNEXPECTED_TOKEN, "'|'"), false);
	if (!check_operator_before_pipe(prev, curr))
		return (false);
	return (true);
}

bool	pipes(t_token *tokenize)
{
	t_pipe		pipe;

	pipe = (t_pipe){tokenize, NULL, false};
	while (pipe.curr)
	{
		if (pipe.curr->type == TOKEN_WORD
			&& is_only_whitespaces(pipe.curr->value))
		{
			pipe.prev = pipe.curr;
			pipe.curr = pipe.curr->next;
			continue ;
		}
		if (pipe.curr->type == TOKEN_PIPE)
		{
			if (!validate_pipe_position(pipe.prev, pipe.curr))
				return (false);
			pipe.expecting_command = true;
		}
		pipe.prev = pipe.curr;
		pipe.curr = pipe.curr->next;
	}
	return (true);
}
