/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:06:42 by ldurmish          #+#    #+#             */
/*   Updated: 2025/02/26 00:38:27 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	ft_is_operator(char c)
{
	return (c == '|' || c == '&' || c == ';');
}

bool	ft_is_logical_op(char current, char next)
{
	return ((current == '&' && next == '&')
		|| (current == '|' && next == '|'));
}
