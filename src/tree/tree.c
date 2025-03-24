/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:32:24 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/24 14:38:01 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast		*left;
	t_ast		*right;
	t_token		*curr;

	(void)left;
	(void)right;
	(void)curr;

	if (!tokens || !*tokens)
		return (NULL);
	// TODO: Implement pipeline parsing logic here
	return (NULL);
}

t_ast	*parse_logic_sequence(t_token **tokens)
{
	t_ast		*left;
	t_ast		*right;
	t_token		*current;
	(void)right;
	(void)current;
	
	if (!tokens || !*tokens)
		return (NULL);
	left = parse_pipeline(tokens);
	if (!left)
		return (NULL);
	// TODO: Implement logic sequence parsing (&&, ||) here
	return (left);
}

t_ast	*parse_command_line(t_token **tokens)
{
	if (!tokens || !*tokens)
		return (NULL);
	return (parse_logic_sequence(tokens));
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