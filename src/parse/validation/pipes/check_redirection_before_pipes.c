/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirection_before_pipes.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 21:08:34 by ldurmish          #+#    #+#             */
/*   Updated: 2025/05/04 20:44:05 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	check_is_noclobber_operator(t_token *prev, t_token *curr)
{
	t_token		*next;

	if (prev && is_noclobber_operator(prev, curr))
	{
		next = curr->next;
		while (next && next->type == TOKEN_WHITESPACE)
			next = next->next;
		while (next && next->type == TOKEN_WORD
			&& is_only_whitespaces(next->value))
			next = next->next;
		if (!next || next->type != TOKEN_WORD
			|| is_only_whitespaces(next->value))
		{
			report_error(ERR_SYNTAX, "missing_filename after '>|'");
			return (false);
		}
	}
	return (true);
}

static bool	is_direct_redir_to_pipe(t_token *curr, t_token *head)
{
	t_token	*check;

	check = head;
	while (check != curr)
	{
		if (check->type == TOKEN_REDIRECT_OUT && check->next == curr)
			return (true);
		check = check->next;
	}
	return (false);
}

static bool	missing_filename(t_token *curr, t_token *head)
{
	t_token		*check;
	t_token		*last_redir;
	bool		has_filename;

	check = head;
	last_redir = NULL;
	has_filename = false;
	while (check != curr)
	{
		if (check->type == TOKEN_HEREDOC || check->type == TOKEN_REDIRECT_IN
			|| check->type == TOKEN_REDIRECT_OUT || check->type == TOKEN_APPEND)
		{
			if (check->next == curr)
				last_redir = NULL;
			last_redir = check;
			has_filename = false;
		}
		else if (last_redir && check->type == TOKEN_WORD
			&& !is_only_whitespaces(check->value))
			has_filename = true;
		check = check->next;
	}
	if (last_redir && !has_filename)
		return (false);
	return (true);
}

bool	check_redirection_before_pipe(t_token *curr, t_token *head)
{
	if (is_direct_redir_to_pipe(curr, head))
		return (false);
	if (!missing_filename(curr, head))
	{
		report_error(ERR_SYNTAX, "missing filename before pipes");
		return (false);
	}
	return (true);
}
