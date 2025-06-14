/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:12:46 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/14 23:51:11 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	ft_is_redirection(char c)
{
	return (c == '<' || c == '>');
}

bool	ft_is_redirection_op(char curr, char next)
{
	return ((curr == '>' && next == '>') || (curr == '<' && next == '<'));
}

bool	check_after_redirection(t_token **curr)
{
	if (((*curr)->type == TOKEN_REDIRECT_OUT || (*curr)->type == TOKEN_APPEND
			|| (*curr)->type == TOKEN_REDIRECT_IN
			|| (*curr)->type == TOKEN_HEREDOC) && (*curr)->next
		&& ((*curr)->next->type == TOKEN_OR
			|| (*curr)->next->type == TOKEN_AND))
		return (report_error(ERR_UNEXPECTED_TOKEN, (*curr)->next->value),
			false);
	return (true);
}

bool	validate_next_redirect(t_token **next)
{
	while (*next && (*next)->type == TOKEN_WHITESPACE)
		*next = (*next)->next;
	if (!*next)
		return (report_error(ERR_UNEXPECTED_TOKEN, "newline"), false);
	if ((*next)->type == TOKEN_WORD)
		return (true);
	if ((*next)->type == TOKEN_REDIRECT_IN || (*next)->type == TOKEN_APPEND
		|| (*next)->type == TOKEN_REDIRECT_OUT
		|| (*next)->type == TOKEN_HEREDOC)
		return (true);
	if ((*next)->type == TOKEN_PIPE)
		return (true);
	if ((*next)->type == TOKEN_PAREN_OPEN || (*next)->type == TOKEN_PAREN_CLOSE)
		return (true);
	report_error(ERR_UNEXPECTED_TOKEN, (*next)->value);
	return (false);
}
