/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:01:41 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/19 14:52:05 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

bool	validation_utils(t_token *tokenize)
{
	if (!pipes(tokenize))
	{
		free_tokens(tokenize);
		return (false);
	}
	if (!validate_redirection(tokenize))
	{
		free_tokens(tokenize);
		return (false);
	}
	return (true);
}

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
	if (!validation_utils(tokenize))
		return (false);
	return (true);
}
