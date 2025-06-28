/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 22:25:21 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/22 17:18:20 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parse_setup_tokens(t_token **tokens)
{
	t_token	*curr;

	if (!tokens || !*tokens)
		return (0);
	curr = *tokens;
	skip_tree_whitespaces(&curr);
	*tokens = curr;
	if (!curr)
		return (0);
	return (1);
}

void	free_redirections(t_redir_ls *redir)
{
	t_redir_ls	*next;

	while (redir)
	{
		next = redir->next;
		if (redir->filename)
			free(redir->filename);
		free(redir);
		redir = next;
	}
}

void	free_cmd(t_commands *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	if (cmd->heredoc_delim)
		free(cmd->heredoc_delim);
	if (cmd->redirections)
		free_redirections(cmd->redirections);
	free(cmd);
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->cmd)
		free_cmd(node->cmd);
	free(node);
}
