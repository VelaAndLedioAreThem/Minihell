/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:06:10 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/15 15:06:13 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*   Additional validation functions for multiple redirections                */
/*   Add these to your existing validation or utility files                  */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Validate redirection chain for semantic correctness
 * Check for conflicting redirections, etc.
 * This is optional - set to strict mode to disallow multiple redirections of same type
 */
int	validate_redirection_chain_strict(t_ast *node)
{
	int		has_stdin_redir = 0;
	int		has_stdout_redir = 0;
	int		has_heredoc = 0;
	t_ast	*curr;

	if (!node)
		return (1);
		
	curr = node;
	while (curr)
	{
		if (curr->type == AST_REDIR_IN)
		{
			if (has_stdin_redir || has_heredoc)
			{
				report_error(ERR_SYNTAX, "ambiguous redirect");
				return (0);
			}
			has_stdin_redir = 1;
		}
		else if (curr->type == AST_REDIR_HERDOC)
		{
			if (has_stdin_redir || has_heredoc)
			{
				report_error(ERR_SYNTAX, "ambiguous redirect");
				return (0);
			}
			has_heredoc = 1;
		}
		else if (curr->type == AST_REDIR_OUT || curr->type == AST_REDIR_APPEND)
		{
			if (has_stdout_redir)
			{
				report_error(ERR_SYNTAX, "ambiguous redirect");
				return (0);
			}
			has_stdout_redir = 1;
		}
		
		curr = curr->left;
	}
	
	return (1);
}

/**
 * More permissive validation - allows multiple redirections of same type
 * (like bash does - last one wins)
 */
int	validate_redirection_chain_permissive(t_ast *node)
{
	t_ast	*curr;

	if (!node)
		return (1);
		
	curr = node;
	while (curr)
	{
		// Check that redirection nodes have proper structure
		if (curr->type == AST_REDIR_IN || curr->type == AST_REDIR_OUT ||
			curr->type == AST_REDIR_APPEND || curr->type == AST_REDIR_HERDOC)
		{
			if (!curr->right || !curr->right->cmd || !curr->right->cmd->args || 
				!curr->right->cmd->args[0])
			{
				report_error(ERR_SYNTAX, "redirection missing target");
				return (0);
			}
		}
		
		curr = curr->left;
	}
	
	return (1);
}

/**
 * Count redirections of each type in a chain
 * Useful for debugging or implementing specific policies
 */
void	count_redirections(t_ast *node, int *in_count, int *out_count, 
						int *append_count, int *heredoc_count)
{
	t_ast	*curr;

	if (!node || !in_count || !out_count || !append_count || !heredoc_count)
		return;
		
	*in_count = 0;
	*out_count = 0;
	*append_count = 0;
	*heredoc_count = 0;
	
	curr = node;
	while (curr)
	{
		if (curr->type == AST_REDIR_IN)
			(*in_count)++;
		else if (curr->type == AST_REDIR_OUT)
			(*out_count)++;
		else if (curr->type == AST_REDIR_APPEND)
			(*append_count)++;
		else if (curr->type == AST_REDIR_HERDOC)
			(*heredoc_count)++;
		
		curr = curr->left;
	}
}
