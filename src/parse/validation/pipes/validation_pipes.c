/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_pipes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:22:21 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/15 14:52:20 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

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

bool	check_operator_before_pipe(t_token *prev, t_token *curr)
{
	t_token		*check;
	bool		has_word;

	if (!is_operator_token(prev))
		return (true);
	if (prev->type == TOKEN_REDIRECT_OUT && ft_strcmp(prev->value, ">") == 0
		&& !has_whitespace_between(prev, curr))
		return (true);
	if (prev->type == TOKEN_APPEND && ft_strcmp(prev->value, ">>") == 0
		&& !has_whitespace_between(prev, curr))
		return (true);
	check = prev->next;
	has_word = false;
	while (check && check != curr)
	{
		if (check->type == TOKEN_WORD && !is_only_whitespaces(check->value))
		{
			has_word = true;
			break ;
		}
		check = check->next;
	}
	if (!has_word)
		return (report_error(ERR_SYNTAX, "token '|'"), false);
	return (true);
}

bool	validate_pipe_position(t_token *prev, t_token *curr, t_token *head)
{
	t_token		*next;

	if (!check_redirection_before_pipe(curr, head))
		return (false);
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

bool	has_expecting_command(t_token *curr, bool *expecting_command)
{
	t_token		*next;

	if (curr->type == TOKEN_WORD && is_only_whitespaces(curr->value))
		return (true);
	else if (curr->type == TOKEN_WORD && !is_only_whitespaces(curr->value))
		*expecting_command = false;
	else if (curr->type == TOKEN_REDIRECT_IN || curr->type == TOKEN_REDIRECT_OUT
		|| curr->type == TOKEN_APPEND || curr->type == TOKEN_HEREDOC)
	{
		next = curr->next;
		if (!next)
			return (report_error(ERR_SYNTAX, "n"), false);
		while (next && next->type == TOKEN_WORD
			&& is_only_whitespaces(next->value))
			next = next->next;
		if (next->type == TOKEN_WORD && !is_only_whitespaces(next->value))
			*expecting_command = false;
	}
	else if (curr->type == TOKEN_AND || curr->type == TOKEN_OR
		|| curr->type == TOKEN_PIPE)
	{
		report_error(ERR_SYNTAX, "missing command");
		return (false);
	}
	return (true);
}

bool	pipes(t_token *tokenize)
{
	t_pipe		pipe;

	pipe = (t_pipe){tokenize, NULL, tokenize, false};
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
			if (!validate_pipe_position(pipe.prev, pipe.curr, pipe.head))
				return (false);
			pipe.expecting_command = true;
		}
		else if (pipe.expecting_command
			&& !has_expecting_command(pipe.curr, &pipe.expecting_command))
			return (false);
		pipe.prev = pipe.curr;
		pipe.curr = pipe.curr->next;
	}
	return (return_pipes_mssg(&pipe.expecting_command));
}
