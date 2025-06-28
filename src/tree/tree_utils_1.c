/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:45:55 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/22 17:23:24 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ast	*parse_init_command_node(t_token *token)
{
	t_ast	*cmd_node;

	cmd_node = create_ast_node(AST_COMMAND, token);
	if (!cmd_node)
		return (NULL);
	cmd_node->cmd = create_command_struct();
	if (!cmd_node->cmd)
	{
		free(cmd_node);
		return (NULL);
	}
	cmd_node->cmd->args = malloc(sizeof(char *));
	if (!cmd_node->cmd->args)
	{
		free(cmd_node->cmd);
		free(cmd_node);
		return (NULL);
	}
	cmd_node->cmd->args[0] = NULL;
	return (cmd_node);
}

int	parse_process_redirection(t_token **tokens, t_ast **cmd_node)
{
	if (!*cmd_node)
	{
		*cmd_node = parse_init_command_node(*tokens);
		if (!*cmd_node)
			return (0);
	}
	if (!parse_redirection(tokens, *cmd_node))
	{
		free_ast(*cmd_node);
		*cmd_node = NULL;
		return (0);
	}
	return (1);
}

int	parse_process_word_token(t_token **tokens, t_ast **cmd_node, int *has_cmd)
{
	t_token	*curr;

	curr = *tokens;
	if (!*has_cmd)
	{
		if (!*cmd_node)
		{
			*cmd_node = parse_init_command_node(curr);
			if (!*cmd_node)
				return (0);
		}
		if (!set_command_name(*cmd_node, curr->value))
			return (free_ast(*cmd_node), *cmd_node = NULL, 0);
		*has_cmd = 1;
	}
	else
	{
		if (!add_command_arg(*cmd_node, curr->value))
			return (free_ast(*cmd_node), *cmd_node = NULL, 0);
	}
	*tokens = curr->next;
	return (1);
}

int	parse_command_loop(t_token **tokens, t_ast **cmd_node, int *has_command)
{
	t_token	*curr;

	curr = *tokens;
	skip_tree_whitespaces(&curr);
	*tokens = curr;
	if (!curr)
		return (0);
	if (is_redirection_token(curr->type))
	{
		if (!parse_process_redirection(tokens, cmd_node))
			return (-1);
	}
	else if (curr->type == TOKEN_WORD || curr->type == TOKEN_WILDCARD)
	{
		if (!parse_process_word_token(tokens, cmd_node, has_command))
			return (-1);
	}
	else
		return (0);
	return (1);
}

t_ast	*parse_simple_commands(t_token **tokens)
{
	t_ast	*cmd_node;
	t_token	*curr;
	int		has_command;
	int		result;

	cmd_node = NULL;
	has_command = 0;
	if (!parse_setup_tokens(tokens))
		return (NULL);
	curr = *tokens;
	while (curr)
	{
		result = parse_command_loop(tokens, &cmd_node, &has_command);
		if (result == -1)
		{
			if (cmd_node)
				free_ast(cmd_node);
			return (NULL);
		}
		if (result == 0)
			break ;
		curr = *tokens;
	}
	return (cmd_node);
}
