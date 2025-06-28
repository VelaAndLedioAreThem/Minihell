/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_paren_utils_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:58:24 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/11 19:57:06 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	it_is_pipe(char *input, int *i, int *j, t_token *token)
{
	int		k;
	t_token	*next;

	*j = *i + 2;
	while (input[*j] && ft_isspace(input[*j]))
		(*j)++;
	if (!input[*j])
	{
		if (!token->next)
		{
			report_error(ERR_SYNTAX, "pipe without command");
			return (free_stack(token), false);
		}
		next = token->next;
		k = 0;
		while (next->value[k] && ft_isspace(next->value[k]))
			k++;
		if (!next->value[k] || !is_valid_command_char(next->value[k]))
			return (report_error(ERR_SYNTAX, "pipe withou command"),
				free_stack(token), false);
	}
	else if (!is_valid_command_char(input[*j]))
		return (report_error(ERR_SYNTAX, "invalid token after pipe"),
			free_stack(token), false);
	return (true);
}

bool	it_is_logical_op(char *input, int *i, int *j, t_token *token)
{
	t_token	*next;
	int		k;

	*j = *i + 3;
	while (input[*j] && ft_isspace(input[*j]))
		(*j)++;
	if (!input[*j])
	{
		if (!token->next)
		{
			report_error(ERR_SYNTAX, "operator without command");
			return (free_stack(token), false);
		}
		next = token->next;
		k = 0;
		while (next->value[k] && ft_isspace(next->value[k]))
			k++;
		if (!next->value[k] || !is_valid_command_char(next->value[k]))
			return (report_error(ERR_SYNTAX, "operator withoud command"),
				free_stack(token), false);
	}
	else if (!is_valid_command_char(input[*j]))
		return (report_error(ERR_SYNTAX, "invalid token after operator"),
			free_stack(token), false);
	return (true);
}

bool	it_is_log_or_pipe(char *input, int *i, t_token *token)
{
	int		k;
	t_token	*next;

	if (input[*i + 1] == '|')
	{
		if (!token->next)
		{
			report_error(ERR_SYNTAX, "pipe without command");
			return (free_stack(token), false);
		}
		next = token->next;
		k = 0;
		while (next->value[k] && ft_isspace(input[k]))
			k++;
		if (!next->value[k] || !is_valid_command_char(next->value[k]))
		{
			report_error(ERR_SYNTAX, "operator without command");
			return (free_stack(token), false);
		}
	}
	else if (!token->next || token->next->value[0] != input[*i + 1])
		return (report_error(ERR_SYNTAX, "incomplete logical operator"),
			free_stack(token), false);
	return (true);
}
