/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:32:24 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/11 12:45:09 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	*skip_node(t_token **tokens, t_token *curr)
{
	if (!tokens || !*tokens)
		return (NULL);
	curr = *tokens;
	skip_tree_whitespaces(&curr);
	*tokens = curr;
	return (curr);
}

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast		*left;
	t_token		*curr;

	if (!tokens || !*tokens)
		return (NULL);
	curr = *tokens;
	skip_tree_whitespaces(&curr);
	*tokens = curr;
	left = parse_command(tokens);
	if (!left)
		return (NULL);
	curr = *tokens;
	skip_tree_whitespaces(&curr);
	*tokens = curr;
	while (curr && curr->type == TOKEN_PIPE)
	{
		left = parse_pipeline_node(left, tokens);
		if (!left)
			return (NULL);
		curr = *tokens;
		skip_tree_whitespaces(&curr);
		*tokens = curr;
	}
	return (left);
}

t_ast	*init_logic_node(t_ast *left, t_token **tokens)
{
	t_ast_type	node_type;
	t_token		*curr;
	t_ast		*node;

	curr = *tokens;
	if (curr->type == TOKEN_AND)
		node_type = AST_AND;
	else if (curr->type == TOKEN_OR)
		node_type = AST_OR;
	else
		return (NULL);
	node = create_ast_node(node_type, curr);
	if (!node)
		return (free_ast(left), NULL);
	node->left = left;
	if (curr->next)
		curr = curr->next;
	else
		return (free_ast(node), NULL);
	skip_tree_whitespaces(&curr);
	*tokens = curr;
	node->right = parse_pipeline(tokens);
	if (!node->right)
		return (free_ast(node), NULL);
	return (node);
}

t_ast	*parse_logic_sequence(t_token **tokens)
{
	t_ast		*left;
	t_ast		*node;
	t_token		*current;

	current = *tokens;
	skip_tree_whitespaces(&current);
	*tokens = current;
	left = parse_pipeline(tokens);
	if (!left)
		return (NULL);
	current = *tokens;
	skip_tree_whitespaces(&current);
	*tokens = current;
	while (current && (current->type == TOKEN_AND || current->type == TOKEN_OR))
	{
		node = init_logic_node(left, tokens);
		if (!node)
			return (NULL);
		left = node;
		current = *tokens;
		skip_tree_whitespaces(&current);
		*tokens = current;
	}
	return (left);
}

t_ast	*parse_tokens(t_token *tokens)
{
	t_token		*curr;
	t_ast		*root;

	if (!tokens)
		return (NULL);
	curr = tokens;
	skip_tree_whitespaces(&curr);
	root = parse_command_line(&curr);
	skip_tree_whitespaces(&curr);
	if (curr && curr->type != TOKEN_EOF)
	{
		free_ast(root);
		return (NULL);
	}
	return (root);
}
