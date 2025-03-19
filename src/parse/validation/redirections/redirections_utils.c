/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:12:46 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/17 23:29:52 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	ft_is_redirection(char c)
{
	return (c == '<' || c == '>');
}

bool	ft_is_redirection_op(char curr, char next)
{
	return ((curr == '>' && curr == '>') || (curr == '<' && next == '<'));
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
