/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 01:24:50 by ldurmish          #+#    #+#             */
/*   Updated: 2025/02/25 18:05:16 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

static bool	validate_open_paren(char *input, int i, t_token *token)
{
	if (i == 0)
		return (true);
	if (i > 0)
	{
		if (is_command_or_arg_char(input[i - 1]))
		{
			report_error(ERR_SYNTAX, "missing operator or space before '(");
			return (free_stack(token), false);
		}
	}
	return (true);
}

static bool	validate_command_paren(char *input, int i, t_paren *command)
{
	command->j = i - 1;
	while (command->j >= 0 && ft_isspace(input[command->j]))
		command->j--;
	if (command->j >= 0 && is_command_or_arg_char(input[command->j]))
	{
		while (command->j >= 0 && is_command_or_arg_char(input[command->j]))
			command->j--;
		command->has_cmd_before = true;
	}
	if (command->has_cmd_before)
	{
		command->j++;
		command->has_operator = false;
		while (command->j < i && !command->has_operator)
		{
			if (ft_is_operator(input[command->j]))
				command->has_operator = true;
			command->j++;
		}
		if (!command->has_operator)
			return (false);
	}
	return (true);
}

static bool	process_open_paren(t_token *token, char *input, int i,
	t_paren *commands)
{
	if (i > 0 && !validate_command_paren(input, i, commands))
	{
		report_error(ERR_SYNTAX, "invalid command before parenthesis");
		return (free_stack(token), false);
	}
	if (!validate_open_paren(input, i, token))
		return (false);
	return (true);
}

bool	check_parenthesis(t_token *token, char *input, int i,
	t_quotes *quote)
{
	t_paren		commands;
	int			new_pos;

	commands = (t_paren){false, false, '\0', false, 0, false};
	if (!quote->in_single_quotes && !quote->in_double_quotes)
	{
		if (!commands.has_commands && is_valid_command_char(input[i]))
			commands.has_commands = true;
		else if (commands.has_commands && ft_isspace(input[i]))
			commands.has_commands = false;
		if (input[i] == '(')
		{
			new_pos = process_open_paren(token, input, i, &commands);
			if (new_pos <= 0)
				return (false);
			i = new_pos;
		}
	}
	return (true);
}
