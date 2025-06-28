/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 21:09:52 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/09 18:13:55 by ldurmish         ###   ########.fr       */
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

bool	valid_before_open_paren(char c)
{
	return (ft_isspace(c) || c == '|' || c == '&' || c == '('
		|| c == ';' || c == '=' || c == '\0');
}

bool	is_valid_after_open_paren(char c)
{
	return (ft_isalnum(c) || c == '(' || c == '$');
}

bool	is_valid_close_paren(char c)
{
	return (ft_isspace(c) || c == '|' || c == '&' || c == ')' || c == ';'
		|| c == '\0');
}

bool	skip_whitespaces(char *input, int *i, int end)
{
	while (*i < end && ft_isspace(input[*i]))
		(*i)++;
	return (*i >= end);
}
