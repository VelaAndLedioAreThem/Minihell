/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_wildcards.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:04:56 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/11 10:45:09 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_word_token(t_expand_wild *exp, char ***temp_args)
{
	char		*token_value;

	token_value = exp->curr->value;
	if (ft_strchr(token_value, '\'') && ft_strncmp(token_value, "'$", 2) == 0)
		(*temp_args)[exp->i] = ft_strndup(token_value + 1,
				ft_strlen(token_value) - 2);
	else
		(*temp_args)[exp->i] = ft_strdup(token_value);
	if (!(*temp_args)[exp->i])
		return (0);
	exp->curr = exp->curr->next;
	exp->i++;
	return (1);
}

static int	process_collect_loop(t_expand_wild *exp, char **temp_args,
	int word_count)
{
	while (exp->curr && exp->i < word_count)
	{
		skip_tree_whitespaces(&exp->curr);
		if (!exp->curr)
			break ;
		if (exp->curr->type == TOKEN_PAREN_OPEN)
		{
			if (!handle_paren_token(exp, &temp_args))
				return (0);
		}
		else if (exp->curr->type == TOKEN_WILDCARD
			|| exp->curr->type == TOKEN_WORD)
		{
			if (!handle_word_token(exp, &temp_args))
			{
				free_2darray(temp_args);
				return (0);
			}
		}
		else
			exp->curr = exp->curr->next;
	}
	return (1);
}

static char	**collect_temp_args(t_token *start, int word_count, int *arg_count)
{
	t_expand_wild	exp;
	char			**temp_args;

	temp_args = malloc(sizeof(char *) * (word_count + 1));
	if (!temp_args)
		return (NULL);
	ft_memset(&exp, 0, sizeof(t_expand_wild));
	exp.curr = start;
	if (!process_collect_loop(&exp, temp_args, word_count))
		return (NULL);
	temp_args[exp.i] = NULL;
	if (arg_count)
		*arg_count = exp.i;
	return (temp_args);
}


t_ast	*create_command_node(t_token *start, int word_count)
{
	t_ast		*node;
	char		**temp_args;
	int			arg_count;

	temp_args = collect_temp_args(start, word_count, &arg_count);
	if (!temp_args)
		return (NULL);
	node = create_ast_node(AST_COMMAND, start);
	if (!node)
		return (free_2darray(temp_args), NULL);
	node->cmd = create_command_struct();
	if (!node->cmd)
		return (free_2darray(temp_args), NULL);
	node->cmd->args = expand_command_args(temp_args, arg_count);
	if (!node->cmd->args)
	{
		free(node->cmd);
		free(node);
		free_2darray(temp_args);
		return (NULL);
	}
	free_2darray(temp_args);
	return (node);
}
