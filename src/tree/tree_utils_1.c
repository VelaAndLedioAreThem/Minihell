/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:24:44 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/30 17:29:12 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static char	*join_adjacent_words(t_token **curr)
{
	char	*result;
	char	*tmp;
	t_token	*next;

	result = ft_strdup((*curr)->value);
	if (!result)
		return (NULL);
	next = (*curr)->next;
	while (next && next->type != TOKEN_WHITESPACE && (next->type == TOKEN_WORD
			|| next->type == TOKEN_WILDCARD))
	{
		tmp = ft_strjoin(result, next->value);
		free(result);
		if (!tmp)
			return (NULL);
		result = tmp;
		*curr = next;
		next = next->next;
	}
	*curr = (*curr)->next;
	return (result);
}

int	parse_process_word_token(t_token **tokens, t_ast **cmd_node, int *has_cmd)
{
	t_token	*curr;
	char	*value;

	curr = *tokens;
	value = join_adjacent_words(&curr);
	if (!value)
		return (free_ast(*cmd_node), *cmd_node = NULL, 0);
	if (!*has_cmd)
	{
		if (!*cmd_node)
		{
			*cmd_node = parse_init_command_node(*tokens);
			if (!*cmd_node)
				return (free(value), 0);
		}
		if (!set_command_name(*cmd_node, value))
			return (free(value), free_ast(*cmd_node), *cmd_node = NULL, 0);
		*has_cmd = 1;
	}
	else
	{
		if (!add_command_arg(*cmd_node, value))
			return (free(value), free_ast(*cmd_node), *cmd_node = NULL, 0);
	}
	return (*tokens = curr, free(value), 1);
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
