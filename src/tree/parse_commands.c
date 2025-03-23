/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 17:53:52 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/23 17:07:45 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ast	*init_command_node(t_token *start)
{
	t_ast		*node;

	node = create_ast_node(AST_COMMAND, start);
	if (!node)
		return (NULL);
	node->cmd = create_command_struct();
	if (!node->cmd)
	{
		free(node);
		return (NULL);
	}
	node->cmd->args = malloc(sizeof(char *) * 2);
	if (!node->cmd->args)
	{
		free(node->cmd);
		free(node);
		return (NULL);
	}
	return (node);
}

t_ast	*create_command_node(t_token *start, int word_count)
{
	t_ast		*node;
	t_token		*curr;
	int			i;

	node = init_command_node(start);
	if (!node)
		return (NULL);
	curr = start;
	i = 0;
	while (i < word_count)
	{
		skip_tree_whitespaces(&curr);
		if (curr && curr->type == TOKEN_WORD)
		{
			node->cmd->args[i++] = ft_strdup(curr->value);
			curr = curr->next;
		}
		else
			break ;
	}
	node->cmd->args[i] = NULL;
	return (node);
}
