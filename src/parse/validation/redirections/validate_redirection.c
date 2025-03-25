/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_redirection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:11:15 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/19 15:22:54 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	check_redirection(t_token **next)
{
	if (*next && ((*next)->type == TOKEN_APPEND
			|| (*next)->type == TOKEN_REDIRECT_IN
			|| (*next)->type == TOKEN_HEREDOC
			|| (*next)->type == TOKEN_REDIRECT_OUT))
	{
		report_error(ERR_UNEXPECTED_TOKEN, (*next)->value);
		return (false);
	}
	else if (*next && ((*next)->type == TOKEN_AND || (*next)->type == TOKEN_OR))
	{
		report_error(ERR_UNEXPECTED_TOKEN, (*next)->value);
		return (false);
	}
	return (true);
}

bool	check_adjacent_redirection(t_token **curr)
{
	if ((*curr)->type == TOKEN_REDIRECT_IN && (*curr)->next
		&& (*curr)->next->type == TOKEN_REDIRECT_OUT)
	{
		(*curr)->type = TOKEN_READWRITE;
		free((*curr)->value);
		(*curr)->value = ft_strdup("<>");
		(*curr)->next->type = TOKEN_PROCESSED;
		return (true);
	}
	return (true);
}

bool	validate_redirection_spaces(t_token *tokenize)
{
	t_token		*curr;
	t_token		*next;

	curr = tokenize;
	while (curr != NULL)
	{
		if (curr->type == TOKEN_REDIRECT_IN || curr->type == TOKEN_APPEND
			|| curr->type == TOKEN_REDIRECT_OUT || curr->type == TOKEN_HEREDOC)
		{
			next = curr->next;
			if (next && next->type == TOKEN_WHITESPACE)
			{
				next = next->next;
				if (!check_redirection(&next))
					return (false);
			}
		}
		if (!check_after_redirection(&curr))
			return (false);
		curr = curr->next;
	}
	return (true);
}

bool	validate_redirection_combinations(t_token *tokenize)
{
	t_token		*curr;
	t_token		*next;

	curr = tokenize;
	while (curr && curr->next)
	{
		if (curr->type == TOKEN_HEREDOC || curr->type == TOKEN_REDIRECT_OUT
			|| curr->type == TOKEN_APPEND || curr->type == TOKEN_REDIRECT_OUT)
		{
			next = curr->next;
			while (next && next->type == TOKEN_WHITESPACE)
				next = next->next;
			if (next && (next->type == TOKEN_REDIRECT_OUT
					|| next->type == TOKEN_APPEND
					|| next->type == TOKEN_REDIRECT_IN
					|| next->type == TOKEN_HEREDOC))
			{
				report_error(ERR_UNEXPECTED_TOKEN, next->value);
				return (false);
			}
		}
		curr = curr->next;
	}
	return (validate_redirection_spaces(tokenize));
}

bool	validate_redirection(t_token *tokenize)
{
	t_token		*curr;
	t_token		*next;

	curr = tokenize;
	while (curr != NULL)
	{
		if (curr->type == TOKEN_PROCESSED)
		{
			curr = curr->next;
			continue ;
		}
		if (curr->type == TOKEN_APPEND || curr->type == TOKEN_REDIRECT_IN
			|| curr->type == TOKEN_REDIRECT_OUT || curr->type == TOKEN_HEREDOC)
		{
			if (!check_adjacent_redirection(&curr))
				return (false);
			next = curr->next;
			if (!validate_next_redirect(&next, &curr))
				return (true);
		}
		curr = curr->next;
	}
	return (validate_redirection_combinations(tokenize));
}
