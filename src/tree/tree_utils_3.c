/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:27:03 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/30 17:35:41 by ldurmish         ###   ########.fr       */
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

static char	*collect_word_value(t_expand_wild *exp)
{
	char	*tok;
	char	*tmp;

	tok = ft_strdup(exp->curr->value);
	if (!tok)
		return (NULL);
	exp->curr = exp->curr->next;
	while (exp->curr && (exp->curr->type == TOKEN_WORD
			|| exp->curr->type == TOKEN_WILDCARD))
	{
		tmp = ft_strjoin(tok, exp->curr->value);
		free(tok);
		if (!tmp)
			return (NULL);
		tok = tmp;
		exp->curr = exp->curr->next;
	}
	return (tok);
}

int	handle_word_token(t_expand_wild *exp, char ***temp_args)
{
	char	*tok;

	tok = collect_word_value(exp);
	if (!tok)
		return (0);
	if (ft_strchr(tok, '\'') && ft_strncmp(tok, "'$", 2) == 0)
		(*temp_args)[exp->i] = ft_strndup(tok + 1,
				ft_strlen(tok) - 2);
	else
		(*temp_args)[exp->i] = ft_strdup(tok);
	free(tok);
	if (!(*temp_args)[exp->i])
		return (0);
	exp->i++;
	return (1);
}

static int	append_word(t_token *tok, char **res, int *quoted)
{
	char	*tmp;

	tmp = ft_strjoin(*res, tok->value);
	if (!tmp)
		return (0);
	free(*res);
	*res = tmp;
	if (*quoted == 0)
	{
		if (tok->quotes.in_single_quotes)
			*quoted = 1;
		else if (tok->quotes.in_double_quotes)
			*quoted = 2;
	}
	return (1);
}

/* ───────────────────── public API (2/2) ─────────────────── */

int	collect_delimiter(t_token **tokens, char **value, int *quoted)
{
	t_token	*curr;
	char	*res;

	curr = *tokens;
	res = ft_strdup("");
	if (!res)
		return (0);
	*quoted = 0;
	while (curr && curr->type == TOKEN_WORD)
	{
		if (!append_word(curr, &res, quoted))
			return (free(res), 0);
		curr = curr->next;
	}
	*tokens = curr;
	*value = res;
	return (1);
}
