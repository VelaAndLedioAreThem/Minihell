/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_utils_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 02:26:37 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/04 03:13:29 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

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
