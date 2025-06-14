/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_wildcards_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:44:41 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/15 01:28:59 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_expanded_wildcard(char **args)
{
	t_expand_wild	exp;
	int				count;

	ft_memset(&exp, 0, sizeof(t_expand_wild));
	count = 0;
	exp.i = -1;
	while (args[++exp.i] != NULL)
	{
		if (has_wildcard(args[exp.i]))
		{
			exp.expanded = expand_wildcard(args[exp.i]);
			if (exp.expanded)
			{
				exp.j = -1;
				while (exp.expanded[++exp.j])
					count++;
				free_matches_array(exp.expanded);
			}
			else
				count++;
		}
		else
			count++;
	}
	return (count);
}

static void	fill_expand_args(char **temp_args, int temp_count,
	t_expand_wild *exp)
{
	exp->i = 0;
	exp->k = 0;
	while (exp->i < temp_count)
	{
		if (has_wildcard(temp_args[exp->i]))
		{
			exp->expanded = expand_wildcard(temp_args[exp->i]);
			if (exp->expanded)
			{
				exp->j = 0;
				while (exp->expanded[exp->j])
					exp->final_args[exp->k++]
						= ft_strdup(exp->expanded[exp->j++]);
				free_2darray(exp->expanded);
			}
			else
				exp->final_args[exp->k++] = ft_strdup(temp_args[exp->i]);
		}
		else
			exp->final_args[exp->k++] = ft_strdup(temp_args[exp->i]);
		exp->i++;
	}
	exp->final_args[exp->k] = NULL;
}

char	**expand_command_args(char **temp_args, int temp_count)
{
	t_expand_wild	exp;
	int				final_count;

	if (!temp_args)
		return (NULL);
	final_count = count_expanded_wildcard(temp_args);
	if (final_count <= 0)
		return (NULL);
	exp.final_args = malloc(sizeof(char *) * (final_count + 1));
	if (!exp.final_args)
		return (NULL);
	fill_expand_args(temp_args, temp_count, &exp);
	return (exp.final_args);
}

static int	is_command_like(t_token *token)
{
	while (token && token->type == TOKEN_WHITESPACE)
		token = token->next;
	if (!token)
		return (0);
	if (token->type == TOKEN_WORD)
	{
		token = token->next;
		while (token && token->type == TOKEN_WHITESPACE)
			token = token->next;
		if (token && (token->type == TOKEN_PIPE
				|| token->type == TOKEN_REDIRECT_OUT
				|| token->type == TOKEN_REDIRECT_IN
				|| token->type == TOKEN_APPEND
				|| token->type == TOKEN_HEREDOC))
			return (1);
		if (token && token->type == TOKEN_WORD)
			return (1);
	}
	return (0);
}

t_ast	*parse_command(t_token **tokens)
{
	t_token		*current;
	t_token		*lookahead;

	if (!tokens || !*tokens)
		return (NULL);
	current = *tokens;
	skip_tree_whitespaces(&current);
	*tokens = current;
	if (current->type == TOKEN_PAREN_OPEN)
	{
		lookahead = current->next;
		skip_tree_whitespaces(&lookahead);
		if (lookahead && is_command_like(lookahead))
			return (parse_subshell(tokens));
		return (parse_simple_commands(tokens));
	}
	return (parse_simple_commands(tokens));
}
