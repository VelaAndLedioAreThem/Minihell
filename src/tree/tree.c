/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:32:24 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/20 17:29:05 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ast	*parse_subshell(t_token **tokens)
{
	t_ast		*node;
	t_token		*curr;

	if (!tokens || !*tokens)
		return (NULL);
	curr = *tokens;
	if (curr->type != TOKEN_PAREN_OPEN)
		return (NULL);
	node = create_ast_node(AST_SUBSHELL, curr);
	if (!node)
		return (NULL);
	curr = curr->next;
	*tokens = curr;
	node->left = parse_command_line(tokens);
	if (!node->left)
		return (free(node), NULL);
	curr = *tokens;
	if (!curr || curr->type != TOKEN_PAREN_CLOSE)
	{
		free_ast(node);
		return (NULL);
	}
	*tokens = curr->next;
	return (node);
}

t_ast	*parse_command(t_token **tokens)
{
	t_token		*current;

	if (!tokens || !*tokens)
		return (NULL);
	current = *tokens;
	if (current->type == TOKEN_PAREN_OPEN)
		return (parse_subshell(tokens));
	return (parse_simple_commands(tokens));
}

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast		*left;
	t_ast		*right;
	t_token		*curr;

	if (!tokens || !*tokens)
		return (NULL);
	left = parse_command(tokens);
	if (!left)
		return (NULL);
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
