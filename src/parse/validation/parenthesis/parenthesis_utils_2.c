/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_utils_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 02:26:37 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/09 18:15:42 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	contains_assignment(const char *value, int *equal_pos)
{
    t_quotes quotes = {false, false};
    int i = 0;
    char prev_char;
    
    if (!value)
        return (false);
    
    while (value[i])
    {
        if (i > 0)
            prev_char = value[i - 1];
        else
            prev_char = '\0';
            
        process_quotes_enhanced(value[i], prev_char, &quotes);
        
        if (value[i] == '=' && !is_in_quotes(&quotes))
        {
            if (equal_pos)
                *equal_pos = i;
            return (true);
        }
        i++;
    }
    return (false);
}

bool	validate_redirect_in_paren(char *input, t_open_paren *paren, int end,
	t_token *token)
{
	if (ft_is_redirection(input[paren->i]))
	{
		if (paren->i + 1 < end && input[paren->i] == input[paren->i + 1])
			paren->i += 2;
		else
			paren->i++;
		while (paren->i < end && ft_isspace(input[paren->i]))
			paren->i++;
		if (paren->i >= end || !is_valid_command_char(input[paren->i]))
		{
			report_error(ERR_SYNTAX, "redirection without filenamd");
			free_stack(token);
			return (false);
		}
		while (paren->i < end && is_valid_command_char(input[paren->i]))
			paren->i++;
	}
	return (true);
}

bool	validate_paren_content_utils(t_open_paren *paren, t_token *token)
{
	if (paren->expecting_commands && !paren->has_commands)
	{
		report_error(ERR_SYNTAX, "missing command after operator");
		free_stack(token);
		return (false);
	}
	return (true);
}

bool	check_closed_paren(char *input, int i, t_token *token)
{
	if (!ft_isspace(input[i + 1]) && !is_valid_close_paren(input[i + 1])
		&& !ft_is_redirection(input[i + 1]) && !ft_is_wildcard(input[i + 1]))
	{
		report_error(ERR_SYNTAX, "invalid token after ')'");
		return (free_stack(token), false);
	}
	if (ft_is_redirection_op(input[i], input[i + 1]))
		return (true);
	if (ft_is_wildcard(input[i + 1]) && !ft_isspace(input[i + 1]))
	{
		report_error(ERR_SYNTAX, "missing space after ')'");
		return (free_stack(token), false);
	}
	return (true);
}

bool	process_close_paren(char *input, int i, t_token *token,
	t_paren *command)
{
	if (!command->has_content)
	{
		if (peek(token) != '(')
		{
			report_error(ERR_SYNTAX, "empty parenthesis");
			return (free_stack(token), false);
		}
	}
	if (input[i + 1])
	{
		if (is_command_or_arg_char(input[i + 1]))
		{
			report_error(ERR_SYNTAX, "missing operator or space after ')'");
			return (free_stack(token), false);
		}
		if (!check_closed_paren(input, i, token))
			return (false);
	}
	else if (!check_next_token(token->next))
		return (false);
	return (true);
}
