/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 01:24:50 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/19 14:15:48 by ldurmish         ###   ########.fr       */
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
		if (ft_is_redirection(input[i - 1]) || ft_is_wildcard(input[i - 1]))
		{
			report_error(ERR_SYNTAX, "invalid token before the '('");
			return (false);
		}
		if (!valid_before_open_paren(input[i - 1]))
		{
			report_error(ERR_SYNTAX, "invalid token before '('");
			return (false);
		}
	}
	if (input[i + 1] && !is_valid_after_open_paren(input[i + 1]))
		return (report_error(ERR_SYNTAX, "invalid token after '('"), false);
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

int	check_matching_paren(char *input, int i, t_token *token, int start)
{
	t_token		*next_token;
	int			end_pos;

	if (i + 1 >= (int)ft_strlen(input))
	{
		start = 0;
		next_token = token->next;
		if (!next_token)
		{
			report_error(ERR_SYNTAX, "unmatched parenthesis");
			return (-1);
		}
		end_pos = find_matching_paren(next_token, next_token->value, start);
		if (end_pos != -1)
			return (end_pos + ft_strlen(input));
	}
	else
	{
		start = i + 1;
		end_pos = find_matching_paren(token, input, start);
		return (end_pos);
	}
	return (-1);
}

static int	process_open_paren(t_token *token, char *input, int i,
	t_paren *commands)
{
	int			start_pos;
	int			end_pos;

	if (i > 0 && !validate_command_paren(input, i, commands))
	{
		report_error(ERR_SYNTAX, "invalid command before parenthesis");
		return (free_stack(token), false);
	}
	if (!validate_open_paren(input, i, token))
		return (-1);
	start_pos = 0;
	end_pos = check_matching_paren(input, i, token, start_pos);
	if (end_pos == -1)
		return (report_error(ERR_SYNTAX, "unclosed parenthesis"), false);
	if (end_pos == i + 1)
		return (report_error(ERR_SYNTAX, "empty parenthesis"), false);
	if (!validate_paren_content(input, start_pos, end_pos, token))
		return (false);
	push(token, '(');
	commands->has_content = true;
	commands->last_op = ')';
	commands->has_commands = false;
	return (end_pos);
}

bool	check_parenthesis(t_token *token, char *input, int i,
	t_paren *commands)
{
	int			new_pos;

	if (!commands->quote.in_single_quotes
		&& !commands->quote.in_double_quotes)
	{
		if (!commands->has_commands && is_valid_command_char(input[i]))
			commands->has_commands = true;
		else if (commands->has_commands && ft_isspace(input[i]))
			commands->has_commands = false;
		if (input[i] == '(')
		{
			new_pos = process_open_paren(token, input, i, commands);
			if (new_pos <= 0)
				return (false);
			i = new_pos;
		}
		else if (input[i] == ')')
		{
			if (!process_close_paren(input, i, token, commands))
				return (false);
		}
	}
	return (true);
}
