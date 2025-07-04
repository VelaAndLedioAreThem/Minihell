/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 22:06:24 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/22 17:11:35 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	validate_redirection_tokens(t_token **tokens, t_token **redir_token,
	t_token **filename_token)
{
	if (!tokens || !*tokens)
		return (0);
	*redir_token = *tokens;
	if ((*redir_token)->next)
		*tokens = (*redir_token)->next;
	else
	{
		printf("bash: syntax error near unexpected token `newline'\n");
		return (0);
	}
	skip_tree_whitespaces(tokens);
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		printf("bash: syntax error near unexpected token\n");
		return (0);
	}
	*filename_token = *tokens;
	return (1);
}

static int	handle_single_redirection(t_token **tokens, t_commands *cmd)
{
	t_token	*redir_token;
	t_token	*filename_token;

	if (!tokens || !*tokens || !cmd)
		return (0);
	if (!validate_redirection_tokens(tokens, &redir_token, &filename_token))
		return (0);
	if (!add_redirection(cmd, redir_token->type, filename_token->value))
		return (0);
	*tokens = (*tokens)->next;
	return (1);
}

int	parse_redirection(t_token **tokens, t_ast *cmd_node)
{
	t_token	*curr;

	if (!tokens || !*tokens || !cmd_node || !cmd_node->cmd)
		return (0);
	curr = *tokens;
	while (curr && is_redirection_token(curr->type))
	{
		if (!handle_single_redirection(&curr, cmd_node->cmd))
			return (0);
		skip_tree_whitespaces(&curr);
	}
	*tokens = curr;
	return (1);
}

int	add_command_arg(t_ast *cmd_node, char *arg)
{
	int		argc;
	char	**new_args;

	argc = 0;
	if (!cmd_node || !cmd_node->cmd || !cmd_node->cmd->args || !arg)
		return (0);
	while (cmd_node->cmd->args[argc])
		argc++;
	new_args = ft_realloc(cmd_node->cmd->args, sizeof(char *) * (argc + 1),
			sizeof(char *) * (argc + 2));
	if (!new_args)
		return (0);
	cmd_node->cmd->args = new_args;
	cmd_node->cmd->args[argc] = ft_strdup(arg);
	cmd_node->cmd->args[argc + 1] = NULL;
	if (!cmd_node->cmd->args[argc])
		return (0);
	return (1);
}

int	set_command_name(t_ast *cmd_node, char *name)
{
	int		i;

	if (!cmd_node || !cmd_node->cmd || !name)
		return (0);
	if (cmd_node->cmd->args)
	{
		i = -1;
		while (cmd_node->cmd->args[++i])
			free(cmd_node->cmd->args[i]);
		free(cmd_node->cmd->args);
	}
	cmd_node->cmd->args = malloc(sizeof(char *) * 2);
	if (!cmd_node->cmd->args)
		return (0);
	cmd_node->cmd->args[0] = ft_strdup(name);
	cmd_node->cmd->args[1] = NULL;
	if (!cmd_node->cmd->args[0])
	{
		free(cmd_node->cmd->args);
		cmd_node->cmd->args = NULL;
		return (0);
	}
	return (1);
}
