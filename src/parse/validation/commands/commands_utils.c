/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:08:52 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/15 18:57:55 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	is_valid_command_char(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '-' || c == '.' || c == '/'
		|| c == '$' || c == '*' || c == '?' || c == '~');
}

bool	is_command_or_arg_char(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '-' || c == '.' || c == '/'
		|| c == '$' || c == '~' || c == '"' || c == '\'');
}

bool	is_valid_command_start(char c)
{
	return (ft_isalnum(c) || c == '.' || c == '/' || c == '$' || c == '?'
		|| c == '#' || c == '_' || c == '*');
}

bool	is_valid_flag_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}