/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:54:09 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/21 20:26:39 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	looks_like_subshell(t_token *curr)
{
	t_token		*next;

	while (curr && curr->type != TOKEN_PAREN_CLOSE)
	{
		if (curr->type == TOKEN_WORD)
		{
			next = curr->next;
			skip_tree_whitespaces(&next);
			if (next && (next->type == TOKEN_PAREN_CLOSE
					|| next->type == TOKEN_PIPE || next->type == TOKEN_WORD
					|| next->type == TOKEN_AND || next->type == TOKEN_OR
					|| next->type == TOKEN_REDIRECT_IN
					|| next->type == TOKEN_APPEND
					|| next->type == TOKEN_HEREDOC
					|| next->type == TOKEN_REDIRECT_OUT))
				return (1);
			if (next && next->type == TOKEN_PAREN_CLOSE)
				return (1);
		}
		else if (curr->type == TOKEN_PIPE || curr->type == TOKEN_AND
			|| curr->type == TOKEN_OR)
			return (1);
		curr = curr->next;
	}
	return (0);
}

char	*append_token_result(char *result, char *token_value)
{
	char	*tmp;

	if (ft_strlen(result) > 0)
	{
		tmp = ft_strjoin(result, " ");
		free(result);
		if (!tmp)
			return (NULL);
		result = tmp;
	}
	tmp = ft_strjoin(result, token_value);
	free(result);
	return (tmp);
}

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
	cmd->redirections = NULL;
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
