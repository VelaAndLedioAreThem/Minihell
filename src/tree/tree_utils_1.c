/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:45:55 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/20 18:23:40 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_commands	*create_command_struct(void)
{
	t_commands		*cmd;

	cmd = malloc(sizeof(t_commands));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->heredoc_delim = NULL;
	cmd->append = 0;
	cmd->is_builtin = 0;
	return (cmd);
}

t_ast	*parse_command_words(t_token **tokens)
{
	t_ast		*cmd_node;
	t_token		*curr;
	t_token		*start;
	int			word_count;

	if (!tokens || !*tokens)
		return (NULL);
	curr = *tokens;
	start = curr;
	word_count = 0;
	while (curr && curr->type == TOKEN_WORD)
	{
		word_count++;
		curr = curr->next;
	}
	if (word_count == 0)
		return (NULL);
	cmd_node = create_command_node(start, word_count);
	if (!cmd_node)
		return (NULL);
	*tokens = curr;
	return (cmd_node);
}

t_ast	*parse_redirection(t_token **tokens, t_ast *cmd_node)
{
	t_ast		*node;
	t_token		*curr;
	t_ast_type	node_type;

	if (!tokens || !*tokens || !cmd_node)
		return (NULL);
	curr = *tokens;
	return (node);
}

t_ast	*parse_simple_commands(t_token **tokens)
{
	t_ast		*redir_node;
	t_ast		*cmd_node;
	t_token		*curr;

	cmd_node = parse_command_words(tokens);
	if (!cmd_node)
		return (NULL);
	curr = *tokens;
	while (curr && (curr->type == TOKEN_APPEND
			|| curr->type == TOKEN_REDIRECT_IN || curr->type == TOKEN_HEREDOC
			|| curr->type == TOKEN_REDIRECT_OUT))
	{
		redir_node = parse_redirection(tokens, cmd_node);
		if (!redir_node)
		{
			free_ast(cmd_node);
			return (NULL);
		}
		cmd_node = redir_node;
		curr = *tokens;
	}
	return (cmd_node);
}
