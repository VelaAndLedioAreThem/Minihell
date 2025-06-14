/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:45:55 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/15 00:17:30 by ldurmish         ###   ########.fr       */
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

t_ast	*init_subshell_node(t_token **tokens)
{
	t_token		*curr;
	t_ast		*node;

	if (!tokens || !*tokens)
		return (NULL);
	curr = *tokens;
	skip_tree_whitespaces(&curr);
	*tokens = curr;
	if (curr->type != TOKEN_PAREN_OPEN)
		return (NULL);
	node = create_ast_node(AST_SUBSHELL, curr);
	if (!node)
		return (NULL);
	*tokens = curr->next;
	skip_tree_whitespaces(tokens);
	return (node);
}

t_ast	*parse_subshell(t_token **tokens)
{
	t_ast		*node;
	t_token		*curr;

	node = init_subshell_node(tokens);
	if (!node)
		return (NULL);
	node->left = parse_command_line(tokens);
	if (!node->left)
	{
		free(node);
		return (NULL);
	}
	curr = *tokens;
	skip_tree_whitespaces(&curr);
	*tokens = curr;
	if (!curr || curr->type != TOKEN_PAREN_CLOSE)
	{
		free_ast(node);
		return (NULL);
	}
	*tokens = curr->next;
	return (node);
}

t_ast	*parse_command_words(t_token **tokens)
{
	t_ast		*cmd_node;
	t_token		*curr;
	t_token		*start;
	int			word_count;

	curr = *tokens;
	if (!skip_node(tokens, curr))
		return (NULL);
	start = curr;
	word_count = 0;
	while (curr && (curr->type == TOKEN_WORD || curr->type == TOKEN_WILDCARD))
	{
		word_count++;
		curr = curr->next;
		skip_tree_whitespaces(&curr);
	}
	if (word_count == 0)
		return (NULL);
	cmd_node = create_command_node(start, word_count);
	if (!cmd_node)
		return (NULL);
	*tokens = curr;
	return (cmd_node);
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
		skip_tree_whitespaces(&curr);
		*tokens = curr;
	}
	return (cmd_node);
}
