/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 01:08:32 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/09 18:12:40 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	ft_is_commands_position(char *input, int i)
{
	int			pos;

	pos = i - 1;
	while (pos >= 0 && ft_isspace(input[pos]))
		pos--;
	if (pos < 0 || input[pos] == '|' || input[pos] == ';' || input[pos] == '('
		|| pos == '\n')
		return (true);
	if (pos > 0 && ((input[pos] == '&' && input[pos - 1] == '&')
			|| (input[pos] == '|' && input[pos - 1] == '|')))
		return (true);
	return (false);
}

static bool	check_commands_in_str(char *str, bool *in_cmd, bool *is_cmd_pos,
	int i)
{
	if (!*in_cmd && is_valid_command_char(str[i])
		&& !ft_isspace(str[i]))
	{
		*is_cmd_pos = ft_is_commands_position(str, i);
		if (*is_cmd_pos && !is_valid_command_start(str[i]))
		{
			report_error(ERR_SYNTAX, "invalid command");
			return (false);
		}
		*in_cmd = true;
	}
	else if (*in_cmd && (ft_isspace(str[i]) || str[i] == '('
			|| str[i] == ')' || ft_is_operator(str[i])
			|| ft_is_redirection(str[i])))
		*in_cmd = false;
	return (true);
}

bool	check_commands(char *str)
{
	int			i;
	t_quotes	quote;
	bool		in_commands;
	bool		is_cmd_pos;

	i = 0;
	quote = (t_quotes){false, false};
	in_commands = false;
	while (str && str[i])
	{
		process_quotes(str[i], &quote);
		if (!quote.in_single_quotes && !quote.in_double_quotes)
		{
			if (!check_commands_in_str(str, &in_commands, &is_cmd_pos, i))
				return (false);
		}
		i++;
	}
	if (quote.in_double_quotes || quote.in_single_quotes)
	{
		report_error(ERR_SYNTAX, "Unmatched quotes");
		return (false);
	}
	return (true);
}

bool	validate_commands(t_token *tokenize)
{
	t_token			*current;

	if (!tokenize)
		return (true);
	current = tokenize;
	while (current)
	{
		if (!check_commands(current->value))
			return (false);
		current = current->next;
	}
	return (true);
}
