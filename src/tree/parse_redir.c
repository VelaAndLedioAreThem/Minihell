/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:52:18 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/30 17:36:10 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	validate_redirection_tokens(t_token **tokens, t_token **redir_token,
		char **filename, int *quoted)
{
	if (!tokens || !*tokens)
		return (0);
	*redir_token = *tokens;
	if ((*redir_token)->next)
		*tokens = (*redir_token)->next;
	else
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
			STDERR_FILENO);
		return (0);
	}
	skip_tree_whitespaces(tokens);
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
			STDERR_FILENO);
		return (0);
	}
	if (!collect_delimiter(tokens, filename, quoted))
		return (0);
	return (1);
}

static int	handle_single_redirection(t_token **tokens, t_commands *cmd)
{
	t_token		*redir_token;
	char		*filename;
	int			quoted;

	if (!tokens || !*tokens || !cmd)
		return (0);
	if (!validate_redirection_tokens(tokens, &redir_token, &filename, &quoted))
		return (0);
	if (!add_redirection(cmd, redir_token->type, filename, quoted))
	{
		free(filename);
		return (0);
	}
	free(filename);
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
	int	i;

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
