/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_logical_operators.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:18:10 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/19 00:17:21 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

static bool	check_sequencing(t_token **curr, t_token **next)
{
	if ((*curr)->type == TOKEN_AND && ft_strcmp((*curr)->value, "&&") == 0
		&& ft_strcmp((*next)->value, ";") == 0)
	{
		report_error(ERR_SYNTAX, "invalid operator sequence &&;");
		return (false);
	}
	if ((*curr)->type == TOKEN_OR && ft_strcmp((*curr)->value, "||") == 0
		&& ft_strcmp((*next)->value, ";") == 0)
	{
		report_error(ERR_SYNTAX, "invalid operator sequence ||;");
		return (false);
	}
	if (ft_strcmp((*curr)->value, ";") == 0 && (*next)->type == TOKEN_OR
		&& ft_strcmp((*next)->value, "||") == 0)
	{
		report_error (ERR_SYNTAX, "invalid operator sequence ;||");
		return (false);
	}
	if (ft_strcmp((*curr)->value, ";") == 0 && (*next)->type == TOKEN_AND
		&& ft_strcmp((*next)->value, "&&") == 0)
		return (report_error(ERR_SYNTAX, "invalid operator sequence ;&&"),
			false);
	return (true);
}

static bool	check_tokens_utils(t_token **curr, t_token **next)
{
	if ((*curr)->type == TOKEN_OR && ft_strcmp((*curr)->value, "||") == 0
		&& (*next)->type == TOKEN_OR && ft_strcmp((*next)->value, "|") == 0)
	{
		report_error(ERR_SYNTAX, "invalid operator sequence |||");
		return (false);
	}
	if ((*curr)->type == TOKEN_AND && ft_strcmp((*curr)->value, "&&") == 0
		&& (*next)->type == TOKEN_OR && ft_strcmp((*next)->value, "||") == 0)
	{
		report_error(ERR_SYNTAX, "invalid operator sequence &&||");
		return (false);
	}
	if ((*curr)->type == TOKEN_OR && ft_strcmp((*curr)->value, "||") == 0
		&& (*next)->type == TOKEN_AND && ft_strcmp((*next)->value, "&&") == 0)
	{
		report_error(ERR_SYNTAX, "invalid operator sequence ||&&");
		return (false);
	}
	return (true);
}

bool	check_tokens(t_token **curr, t_token **next)
{
	if ((*next) && ((*next)->type == TOKEN_AND || (*next)->type == TOKEN_OR
			|| (*next)->type == TOKEN_PIPE))
	{
		if ((*curr)->type == TOKEN_AND && ft_strcmp((*curr)->value, "&&") == 0
			&& (*next)->type == TOKEN_AND
			&& ft_strcmp((*next)->value, "&") == 0)
		{
			report_error(ERR_SYNTAX, "invalid operators sequence '&&&'");
			return (false);
		}
		if ((*curr)->type == TOKEN_OR && ft_strcmp((*curr)->value, "||") == 0
			&& (*next)->type == TOKEN_PIPE)
		{
			report_error(ERR_SYNTAX, "invalid operator '|||'");
			return (false);
		}
		if (!check_tokens_utils(curr, next))
			return (false);
		if (!check_sequencing(curr, next))
			return (false);
	}
	return (true);
}

bool	validate_logical_op(t_token *tokens)
{
	t_token		*next;
	t_token		*curr;

	curr = tokens;
	next = NULL;
	while (curr)
	{
		next = curr->next;
		if (curr->type == TOKEN_AND || curr->type == TOKEN_OR
			|| curr->type == TOKEN_PIPE)
		{
			if (!check_tokens(&curr, &next))
				return (false);
		}
		curr = curr->next;
	}
	return (true);
}
