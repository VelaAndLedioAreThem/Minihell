/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:32:24 by ldurmish          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/03/24 14:38:01 by vszpiech         ###   ########.fr       */
=======
/*   Updated: 2025/03/23 20:06:32 by ldurmish         ###   ########.fr       */
>>>>>>> parsing
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ast	*parse_command(t_token **tokens)
{
	t_token		*current;

	if (!tokens || !*tokens)
		return (NULL);
	current = *tokens;
	skip_tree_whitespaces(&current);
	*tokens = current;
	if (current->type == TOKEN_PAREN_OPEN)
		return (parse_subshell(tokens));
	return (parse_simple_commands(tokens));
}

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast		*left;
	t_token		*curr;

	(void)left;
	(void)right;
	(void)curr;

	if (!tokens || !*tokens)
		return (NULL);
<<<<<<< HEAD
	// TODO: Implement pipeline parsing logic here
	return (NULL);
=======
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
		return (NULL);
	skip_tree_whitespaces(&curr);
	*tokens = curr;
	node->right = parse_pipeline(tokens);
	if (!node->right)
		return (free_ast(left), NULL);
	return (node);
>>>>>>> parsing
}

t_ast	*parse_logic_sequence(t_token **tokens)
{
	t_ast		*left;
	t_ast		*node;
	t_token		*current;
<<<<<<< HEAD
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

=======

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

>>>>>>> parsing
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