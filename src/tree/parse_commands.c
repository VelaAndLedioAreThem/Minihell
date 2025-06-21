/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 17:53:52 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/21 16:22:49 by ldurmish         ###   ########.fr       */
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

t_redir_ls	*create_redir_node(int type, char *filename)
{
	t_redir_ls	*redir;

	redir = malloc(sizeof(t_redir_ls));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->filename = ft_strdup(filename);
	if (!redir->filename)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

int	add_redirection(t_commands *cmd, int type, char *filename)
{
	t_redir_ls	*new_redir;
	t_redir_ls	*curr;

	if (!filename || !*filename)
		return (0);
	new_redir = create_redir_node(type, filename);
	if (!new_redir)
		return (0);
	if (!cmd->redirections)
	{
		cmd->redirections = new_redir;
		return (1);
	}
	curr = cmd->redirections;
	while (curr->next)
		curr = curr->next;
	curr->next = new_redir;
	return (1);
}

int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_APPEND || type == TOKEN_REDIRECT_IN
		|| type == TOKEN_HEREDOC || type == TOKEN_REDIRECT_OUT);
}
