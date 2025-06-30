/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_parenthesis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 09:55:50 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/30 14:04:32 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	validate_paren_depth(t_token *curr, int *max_depth)
{
	int		paren_count;

	paren_count = 1;
	*max_depth = 1;
	while (curr && paren_count > 0)
	{
		if (curr->type == TOKEN_PAREN_OPEN)
		{
			paren_count++;
			if (paren_count > *max_depth)
				*max_depth = paren_count;
			if (*max_depth > 2)
				return (0);
		}
		else if (curr->type == TOKEN_PAREN_CLOSE)
			paren_count--;
		curr = curr->next;
	}
	return (paren_count == 0);
}

static int	validate_paren_token(t_expand_wild *exp)
{
	int		max_depth;

	if (!exp->curr || exp->curr->type != TOKEN_PAREN_OPEN)
		return (0);
	if (!validate_paren_depth(exp->curr->next, &max_depth))
	{
		report_error(ERR_SYNTAX, "nested parentheses too deep");
		return (0);
	}
	return (1);
}

static int	handle_paren_append(t_expand_wild *exp, char **result)
{
	char	*tmp;

	tmp = append_token_result(*result, exp->curr->value);
	if (!tmp)
	{
		free(*result);
		return (0);
	}
	*result = tmp;
	return (1);
}

static char	*extract_paren_token(t_expand_wild *exp)
{
	char	*result;
	int		paren_count;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	exp->curr = exp->curr->next;
	paren_count = 1;
	while (exp->curr && paren_count > 0)
	{
		if (exp->curr->type == TOKEN_PAREN_OPEN)
			paren_count++;
		else if (exp->curr->type == TOKEN_PAREN_CLOSE && --paren_count == 0)
			break ;
		if (paren_count > 0 && !handle_paren_append(exp, &result))
			return (NULL);
		exp->curr = exp->curr->next;
	}
	if (paren_count != 0)
	{
		free(result);
		report_error(ERR_SYNTAX, "unmatched parenthesis");
		return (NULL);
	}
	return (result);
}

int	handle_paren_token(t_expand_wild *exp, char ***temp_args)
{
	char	*result;

	if (!validate_paren_token(exp))
		return (0);
	result = extract_paren_token(exp);
	if (!result)
		return (0);
	if (exp->curr)
		exp->curr = exp->curr->next;
	(*temp_args)[exp->i] = result;
	exp->i++;
	return (1);
}
