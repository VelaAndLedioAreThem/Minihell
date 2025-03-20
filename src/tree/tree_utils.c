/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:23:10 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/20 18:00:04 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ast	*create_command_node(t_token *start, int word_count)
{
	t_ast		*node;
	t_token		*curr;
	int			i;

	node = create_ast_node(AST_COMMAND, start);
	if (!node)
		return (NULL);
	node->cmd = create_command_struct();
	if (!node->cmd)
		return (free(node), NULL);
	node->cmd->args = malloc(sizeof(char *) * (word_count + 1));
	if (!node->cmd->args)
		return (free(node->cmd), free(node), NULL);
	curr = start;
	i = 0;
	while (i < word_count)
	{
		node->cmd->args[i] = ft_strdup(curr->value);
		curr = curr->next;
		i++;
	}
	node->cmd->args[i] = NULL;
	return (node);
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
			free(node->cmd->args[i]);
		}
		if (node->cmd->infile)
			free(node->cmd->infile);
		if (node->cmd->outfile)
			free(node->cmd->outfile);
		if (node->cmd->heredoc_delim)
			free(node->cmd->heredoc_delim);
		free(node->cmd);
	}
}
