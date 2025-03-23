/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 22:06:24 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/21 22:43:47 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ast_type	get_redir_type(t_token *tokens)
{
	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_APPEND)
		return (AST_REDIR_APPEND);
	else if (tokens->type == TOKEN_HEREDOC)
		return (AST_REDIR_HERDOC);
	else if (tokens->type == TOKEN_REDIRECT_IN)
		return (AST_REDIR_IN);
	else if (tokens->type == TOKEN_REDIRECT_OUT)
		return (AST_REDIR_OUT);
	return (0);
}

t_ast	*create_redir(t_token **token, t_ast *cmd_node)
{
	t_token		*curr;
	t_ast		*node;

	curr = *token;
	if (!curr)
		return (NULL);
	node = create_ast_node(get_redir_type(curr), curr);
	if (!node)
		return (NULL);
	node->operator_type = curr->type;
	node->left = cmd_node;
	if (curr->next)
		curr = curr->next;
	else
		return (node);
	skip_tree_whitespaces(&curr);
	*token = curr;
	return (node);
}

t_ast	*create_right_node(t_token **token)
{
	t_ast		*right_node;
	t_token		*curr;

	curr = *token;
	right_node = create_ast_node(AST_COMMAND, curr);
	if (!right_node)
		return (NULL);
	right_node->cmd = create_command_struct();
	if (!right_node)
	{
		free(right_node);
		return (NULL);
	}
	right_node->cmd->args = malloc(sizeof(char *) * 2);
	if (!right_node->cmd->args)
	{
		free(right_node);
		return (NULL);
	}
	right_node->cmd->args[0] = ft_strdup(curr->value);
	right_node->cmd->args[1] = NULL;
	*token = curr->next;
	return (right_node);
}

t_ast	*parse_redirection(t_token **tokens, t_ast *cmd_node)
{
	t_ast		*node;
	t_token		*curr;

	if (!tokens || !*tokens || !cmd_node)
		return (NULL);
	curr = *tokens;
	skip_tree_whitespaces(&curr);
	if (get_redir_type(curr) == 0)
		return (NULL);
	node = create_redir(&curr, cmd_node);
	if (!node || !curr || curr->type != TOKEN_WORD)
	{
		free(node);
		return (NULL);
	}
	node->right = create_right_node(&curr);
	if (node->right)
	{
		free(node);
		return (NULL);
	}
	skip_tree_whitespaces(&curr);
	*tokens = curr;
	return (node);
}
