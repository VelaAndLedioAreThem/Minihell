/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 21:09:52 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/09 00:24:21 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	valid_before_open_paren(char c)
{
	return (ft_isspace(c) || c == '|' || c == '&' || c == '('
		|| c == ';' || c == '\0');
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
