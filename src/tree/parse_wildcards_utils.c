/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_wildcards_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:44:41 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/21 15:02:09 by ldurmish         ###   ########.fr       */
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

static int	is_subshell_context(t_token *token)
{
	t_token	*curr;

	if (!token || token->type != TOKEN_PAREN_OPEN)
		return (0);
	curr = token->next;
	skip_tree_whitespaces(&curr);
	return (looks_like_subshell(curr));
}

t_ast	*parse_command(t_token **tokens)
{
	t_token	*current;

	if (!tokens || !*tokens)
		return (NULL);
	current = *tokens;
	skip_tree_whitespaces(&current);
	*tokens = current;
	if (current->type == TOKEN_PAREN_OPEN)
	{
		if (is_subshell_context(current))
			return (parse_subshell(tokens));
		else
			return (parse_simple_commands(tokens));
	}
	return (parse_simple_commands(tokens));
}
