/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:23:10 by ldurmish          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/06/02 01:07:43 by ldurmish         ###   ########.fr       */
=======
/*   Updated: 2025/06/02 00:38:57 by ldurmish         ###   ########.fr       */
>>>>>>> 962469b0a5eb54f5ee5feddd576b2a74f56fd7b8
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ast	*parse_pipeline_node(t_ast *left, t_token **tokens)
{
	t_token		*curr;
	t_ast		*node;

	curr = *tokens;
	node = create_ast_node(AST_PIPELINE, curr);
	if (!node)
	{
		free_ast(left);
		return (NULL);
	}
	node->left = left;
	if (curr->next)
		curr = curr->next;
	else
		return (node);
	skip_tree_whitespaces(&curr);
	*tokens = curr;
	node->right = parse_command_line(tokens);
	if (!node->right)
	{
		free_ast(left);
		return (NULL);
	}
	return (node);
}

void	skip_tree_whitespaces(t_token **tokens)
{
	while (*tokens && (*tokens)->type == TOKEN_WHITESPACE)
		*tokens = (*tokens)->next;
}

t_ast	*create_ast_node(t_ast_type type, t_token *token)
{
	t_ast		*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->cmd = NULL;
	node->operator_type = 0;
	node->token = token;
	return (node);
}

t_ast	*parse_command_line(t_token **tokens)
{
	return (parse_logic_sequence(tokens));
}

void	free_ast(t_ast *node)
{
	int			i;

	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->cmd)
	{
		if (node->cmd->args)
		{
			i = -1;
			while (node->cmd->args[++i])
				free(node->cmd->args[i]);
			free(node->cmd->args);
		}
		if (node->cmd->infile)
			free(node->cmd->infile);
		if (node->cmd->outfile)
			free(node->cmd->outfile);
		if (node->cmd->heredoc_delim)
			free(node->cmd->heredoc_delim);
		free(node->cmd);
	}
<<<<<<< HEAD
	free (node);
=======
	free(node);
>>>>>>> 962469b0a5eb54f5ee5feddd576b2a74f56fd7b8
}
