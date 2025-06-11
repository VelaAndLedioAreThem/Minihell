/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assignment_paren.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 10:15:35 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/10 13:15:44 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

static bool	is_assignment_token(const char *value, int *equal_pos)
{
	t_quotes	quotes;
	int			i;

	if (!value)
		return (false);
	quotes = (t_quotes){false, false};
	i = -1;
	if (equal_pos)
		*equal_pos = -1;
	while (value[++i])
	{
		if (value[i] == '\'' && !quotes.in_double_quotes)
			quotes.in_single_quotes = !quotes.in_single_quotes;
		else if (value[i] == '"' && !quotes.in_single_quotes)
			quotes.in_double_quotes = !quotes.in_double_quotes;
		else if (value[i] == '=' && !quotes.in_single_quotes
			&& !quotes.in_double_quotes)
		{
			if (equal_pos)
				*equal_pos = i;
			return (true);
		}
	}
	return (false);
}

void	reset_assign(t_assign_context *ctx)
{
	ctx->in_assignment = false;
	ctx->after_equals = false;
	ctx->assign_start = NULL;
	ctx->equal_pos = -1;
}

void	handle_assignment_token(t_assign_context *ctx, t_token *token)
{
	if (is_assignment_token(token->value, &ctx->equal_pos))
	{
		ctx->in_assignment = true;
		ctx->after_equals = true;
		if (!ctx->assign_start)
			ctx->assign_start = token;
	}
	else if (ctx->in_assignment && !ctx->after_equals)
		reset_assign(ctx);
}

bool	is_assignment_command(const char *value)
{
	if (!value)
		return (false);
	if (ft_strcmp(value, "export") == 0)
		return (true);
	if (ft_strcmp(value, "env") == 0)
		return (true);
	if (ft_strcmp(value, "local") == 0)
		return (true);
	if (ft_strcmp(value, "declare") == 0)
		return (true);
	return (false);
}

bool	check_paren_syntax(t_token *curr, t_token *prev, t_assign_context *ctx)
{
	if (curr->type != TOKEN_PAREN_OPEN)
		return (true);
	if (!prev)
		return (true);
	if (ctx->in_assignment && ctx->after_equals)
		return (true);
	return (true);
}
