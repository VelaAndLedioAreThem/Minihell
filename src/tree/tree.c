/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:32:24 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/20 15:16:25 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast		*left;
	t_ast		*right;
	t_token		*curr;

	if (!tokens || !*tokens)
}

t_ast	*parse_logic_sequence(t_token **tokens)
{
	t_ast		*left;
	t_ast		*right;
	t_token		*current;

	if (!tokens || !*tokens)
		return (NULL);
	left = parse_pipeline(tokens);
	if (!left)
		return (NULL);
	return (left);
}

t_ast	*parse_command_line(t_token **curr)
{
	return (parse_loigc_sequence(tokens));
}

t_ast	*parse_tokens(t_token *tokens)
{
	t_token		*curr;
	t_ast		*root;

	if (!tokens)
		return (NULL);
	curr = tokens;
	root = parse_command_line(&curr);
	return (root);
}
