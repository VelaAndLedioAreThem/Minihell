/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:23:10 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/19 01:16:41 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
