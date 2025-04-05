/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_commands_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:10:09 by codespace         #+#    #+#             */
/*   Updated: 2025/04/05 11:34:03 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	handle_dash(char *str, int i, t_flag_state *flag_state)
{
	int		next;

	if (str[i] == '-' && (i == 0 || ft_isspace(str[i - 1])))
	{
		if (str[i + 1] == '-')
		{
			*flag_state = FLAG_DOUBLE_DASH;
			next = i + 2;
			while (str[next] && ft_isspace(str[next]))
				next++;
			if (str[next] && !ft_isspace(str[next]) && str[next] != '|'
				&& str[next] != ';' && str[next] != '&' && !ft_is_redirection(str[next]))
				return (report_error(ERR_SYNTAX, "invalid content after double dash"),
					false);
		}
		*flag_state = FLAG_SINGLE_DASH;
		if (ft_isspace(str[i + 1]))
			return (report_error(ERR_SYNTAX, "invalid flag format (space after dash)"),
			false);
		if (!is_valid_flag_char(str[i + 1]))
			return (report_error(ERR_SYNTAX, "invalid flag character"), false);
	}
	return (true);
}

bool	process_flag_chars(char *str, int i, t_flag_state *flag_state)
{
	if (*flag_state == FLAG_SINGLE_DASH)
	{
		if (!is_valid_flag_char(str[i]))
		{
			report_error(ERR_SYNTAX, "invalid flag character");
			return (false);
		}	
	}
	return (true);
}

bool	process_flag(char *str, int i, t_flag_state *flag_state)
{
	if (!handle_dash(str, i, flag_state))
		return (false);
	return process_flag_chars(str, i, flag_state);
}