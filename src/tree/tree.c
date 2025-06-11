/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:32:24 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/10 13:39:28 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Helper function to determine if token sequence looks like a command */
static int is_command_like(t_token *token)
{
    /* Skip whitespace */
    while (token && token->type == TOKEN_WHITESPACE)
        token = token->next;
    
    if (!token)
        return (0);
    
    /* Check if it starts with a word that could be a command */
    if (token->type == TOKEN_WORD)
    {
        /* Look for command-like patterns: word followed by args, pipes, etc. */
        token = token->next;
        while (token && token->type == TOKEN_WHITESPACE)
            token = token->next;
        
        /* If followed by pipe, redirect, or other operators, likely a command */
        if (token && (token->type == TOKEN_PIPE || 
                     token->type == TOKEN_REDIRECT_OUT ||
                     token->type == TOKEN_REDIRECT_IN ||
                     token->type == TOKEN_APPEND ||
                     token->type == TOKEN_HEREDOC))
            return (1);
        
        /* If followed by more words, could be command with args */
        if (token && token->type == TOKEN_WORD)
            return (1);
    }
    
    return (0);
}

t_ast *parse_command(t_token **tokens)
{
    t_token *current;
    t_token *lookahead;

    if (!tokens || !*tokens)
        return (NULL);
    
    current = *tokens;
    skip_tree_whitespaces(&current);
    *tokens = current;
    
    /* Check if this is actually a subshell or just literal parentheses */
    if (current->type == TOKEN_PAREN_OPEN)
    {
        /* Look ahead to see if this looks like a subshell */
        lookahead = current->next;
        skip_tree_whitespaces(&lookahead);
        
        /* If the content inside parentheses looks like a command, treat as subshell */
        if (lookahead && is_command_like(lookahead))
            return (parse_subshell(tokens));
        
        /* Otherwise, treat parentheses as literal text for echo/commands */
        return (parse_simple_commands(tokens));
    }
    
    return (parse_simple_commands(tokens));
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
