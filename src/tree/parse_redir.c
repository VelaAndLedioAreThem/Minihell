/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 22:06:24 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/21 16:26:21 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	validate_redirection_filename(char *filename)
{
	char	*dir;
	char	*last_slash;

	if (!filename || !*filename)
		return (0);
	if (ft_strchr(filename, '/') && access(filename, F_OK) == -1)
	{
		dir = ft_strdup(filename);
		last_slash = ft_strrchr(dir, '/');
		if (last_slash)
		{
			*last_slash = '\0';
			if (access(dir, W_OK) == -1)
			{
				free(dir);
				return (0);
			}
		}
		free(dir);
	}
	return (1);
}

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
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (0);
	}
	skip_tree_whitespaces(tokens);
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		printf("minishell: syntax error near unexpected token\n");
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
	if (!validate_redirection_filename(filename_token->value))
	{
		printf("minishell: %s: No such file or directory\n",
			filename_token->value);
		return (0);
	}
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
