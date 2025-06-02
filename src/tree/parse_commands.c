/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 17:53:52 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/02 11:05:23 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ast	*init_command_node(t_token *start, int word_count)
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
	node->cmd->args = malloc(sizeof(char *) * (word_count + 1));
	if (!node->cmd->args)
	{
		free(node->cmd);
		free(node);
		return (NULL);
	}
	return (node);
}
