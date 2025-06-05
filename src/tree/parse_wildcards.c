/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_wildcards.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:04:56 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/02 12:01:22 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_expanded_wildcards(char **args)
{
	t_expand_wild	expanded;
	int				count;

	ft_memset(&expanded, 0, sizeof(t_expand_wild));
	count = 0;
	expanded.i = -1;
	while (args[++expanded.i] != NULL)
	{
		if (has_wildcard(args[expanded.i]))
		{
			expanded.expanded = expand_wildcard(args[expanded.i]);
			if (expanded.expanded)
			{
				expanded.j = -1;
				while (expanded.expanded[++expanded.j])
					count++;
				free_matches_array(expanded.expanded);
			}
			else
				count++;
		}
		else
			count++;
	}
	return (count);
}

static char	**collect_temp_args(t_token *start, int word_count, int *arg_count)
{
	t_expand_wild	expanded;

	expanded.temp_args = malloc(sizeof(char *) * (word_count + 1));
	if (!expanded.temp_args)
		return (NULL);
	expanded.i = -1;
	expanded.curr = start;
	while (++expanded.i < word_count)
	{
		skip_tree_whitespaces(&expanded.curr);
		if (expanded.curr && (expanded.curr->type == TOKEN_WORD
				|| expanded.curr->type == TOKEN_WILDCARD))
		{
			expanded.temp_args[expanded.i] = ft_strdup(expanded.curr->value);
			if (!expanded.temp_args[expanded.i])
				return (free_2darray(expanded.temp_args), NULL);
			expanded.curr = expanded.curr->next;
		}
		else
			break ;
	}
	expanded.temp_args[expanded.i] = NULL;
	if (arg_count)
		*arg_count = expanded.i;
	return (expanded.temp_args);
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

static char	**expand_command_args(char **temp_args, int temp_count)
{
	t_expand_wild	exp;
	int				final_count;

	final_count = count_expanded_wildcards(temp_args);
	exp.final_args = malloc(sizeof(char *) * (final_count + 1));
	if (!exp.final_args)
		return (NULL);
	fill_expand_args(temp_args, temp_count, &exp);
	return (exp.final_args);
}

t_ast	*create_command_node(t_token *start, int word_count)
{
	t_ast	*node;
	char	**temp_args;
	int		arg_count;

	temp_args = collect_temp_args(start, word_count, &arg_count);
	if (!temp_args)
		return (NULL);
	node = create_ast_node(AST_COMMAND, start);
	if (!node)
		return (free_2darray(temp_args), NULL);
	node->cmd = create_command_struct();
	if (!node->cmd)
		return (free(node), free_2darray(temp_args), NULL);
	node->cmd->args = expand_command_args(temp_args, arg_count);
	if (!node->cmd->args)
	{
		free(node->cmd);
		free(node);
		return (NULL);
	}
	return (node);
}
