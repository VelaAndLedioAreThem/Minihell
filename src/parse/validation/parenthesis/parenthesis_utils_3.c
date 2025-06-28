/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_utils_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:12:26 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/28 16:32:50 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	check_command_before(char *input, int i, t_token *token)
{
	bool	found_operator;
	bool	found_space;
	int		j;

	if (is_command_or_arg_char(input[i - 1]))
	{
		found_operator = false;
		found_space = false;
		j = i - 1;
		while (j >= 0 && is_command_or_arg_char(input[j]))
			j--;
		while (j >= 0 && ft_isspace(input[j]))
		{
			found_space = true;
			j--;
		}
		if (j >= 0 && (ft_is_operator(input[j]) || input[j] == '|'
				|| input[j] == '&'))
			found_operator = true;
		if (!found_operator && !found_space)
			return (report_error(ERR_SYNTAX,
					"missing operator or space before '('"), free_stack(token),
				false);
	}
	return (true);
}

bool	check_command_paren_sequence(t_token *curr, t_token *prev,
		t_assign_context *ctx)
{
	if (curr->type == TOKEN_PAREN_OPEN && prev && prev->type == TOKEN_WORD
		&& !ctx->in_assignment)
	{
		if (ctx->in_assignment)
			return (true);
		if (!is_assignment_command(prev->value))
		{
			if (curr->next && curr->next->value)
				report_error(ERR_UNEXPECTED_TOKEN, curr->value);
			else
				report_error(ERR_UNEXPECTED_TOKEN, "(");
			return (false);
		}
	}
	return (true);
}

bool	find_command_before_paren(char *input, int i, t_paren *command)
{
	if (!input || i <= 0 || !command)
		return (false);
	command->j = i - 1;
	while (command->j >= 0 && ft_isspace(input[command->j]))
		command->j--;
	if (command->j >= 0 && is_command_or_arg_char(input[command->j]))
	{
		while (command->j >= 0 && is_command_or_arg_char(input[command->j]))
			command->j--;
		command->has_cmd_before = true;
		return (true);
	}
	command->has_cmd_before = false;
	return (true);
}

bool	check_operator_before_command(char *input, t_paren *command)
{
	int	operator_command;

	operator_command = command->j;
	while (operator_command >= 0 && ft_isspace(input[operator_command]))
		operator_command--;
	if (operator_command >= 0)
	{
		if (ft_is_operator(input[operator_command])
			|| input[operator_command] == '|' || input[operator_command] == '&')
		{
			command->has_operator = true;
			return (true);
		}
		if (operator_command > 0 && (input[operator_command] == '|'
				|| input[operator_command] == '|')
			&& (input[operator_command] == '&'
				|| input[operator_command] == '&'))
		{
			command->has_operator = true;
			return (true);
		}
	}
	command->has_operator = false;
	return (true);
}
