/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:01:41 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/04 16:21:29 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

bool	validation(t_token *tokenize)
{
	if (!validate_logical_op(tokenize))
	{
		free_tokens(tokenize);
		return (false);
	}
	if (!validate_commands(tokenize))
	{
		free_tokens(tokenize);
		return (false);
	}
	if (!validation_parenthesis(tokenize))
	{
		free_tokens(tokenize);
		return (false);
	}
	if (!validation_quotes(tokenize))
	{
		free_tokens(tokenize);
		return (false);
	}
	return (true);
}
