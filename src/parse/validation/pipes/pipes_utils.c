/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:23:14 by ldurmish          #+#    #+#             */
/*   Updated: 2025/04/12 21:24:48 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	is_operator_token(t_token *prev)
{
	if (!prev)
		return (false);
	return (prev->type == TOKEN_REDIRECT_IN || prev->type == TOKEN_REDIRECT_OUT
		|| prev->type == TOKEN_APPEND || prev->type == TOKEN_HEREDOC
		|| prev->type == TOKEN_AND || prev->type == TOKEN_OR
		|| prev->type == TOKEN_PIPE);
}

bool	has_whitespace_between(t_token *first, t_token *second)
{
	t_token		*curr;

	curr = first->next;
	while (curr && curr != second)
	{
		if (curr->type == TOKEN_WORD && is_only_whitespaces(curr->value))
			return (true);
		curr = curr->next;
	}
	return (false);
}

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

bool	return_pipes_mssg(bool *expecting_cmd)
{
	if (*expecting_cmd)
	{
		report_error(ERR_SYNTAX, "missing command after pipes");
		return (false);
	}
	return (true);
}
