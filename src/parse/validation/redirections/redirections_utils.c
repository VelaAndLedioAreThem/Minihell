/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:12:46 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/06 20:14:01 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

bool	ft_is_redirection(char c)
{
	return (c == '<' || c == '>');
}

bool	ft_is_redirection_op(char curr, char next)
{
	return ((curr == '>' && curr == '>') || (curr == '<' && next == '<'));
}
