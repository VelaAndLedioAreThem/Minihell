/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_validation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 17:59:54 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/28 19:26:18 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

static bool     is_arithmetic_expression(t_token *token)
{
    t_token *curr;
    int             i;
    bool    found_digit;

    if (!token || !token->next || !token->next->next)
            return (false);
    curr = token->next->next;
    found_digit = false;
    while (curr && !(curr->type == TOKEN_PAREN_CLOSE
                    && curr->next && curr->next->type == TOKEN_PAREN_CLOSE))
    {
            if (curr->type == TOKEN_WORD)
            {
                    i = 0;
                    while (curr->value[i])
                    {
                            if (ft_isalpha(curr->value[i]))
                                    return (false);
                            if (ft_isdigit(curr->value[i]))
                                    found_digit = true;
                            i++;
                    }
            }
            else if (curr->type != TOKEN_WHITESPACE
                            && curr->type != TOKEN_PAREN_OPEN
                            && curr->type != TOKEN_PAREN_CLOSE)
                    return (false);
            curr = curr->next;
    }
    if (!curr || !curr->next)
            return (false);
    return (found_digit);
}

bool	parenthesis(t_token *token, char *input, t_paren *commands,
	t_assign_context *ctx)
{
	int						i;
	t_validation_context	vctx;

	vctx.command = commands;
	vctx.token = token;
	vctx.input = input;
	vctx.ctx = ctx;
	i = 0;
	while (input[i] != '\0')
	{
		if (!check_parenthesis(&vctx, i))
			return (false);
		i++;
	}
	return (true);
}

bool	parenthesis_utils(t_token *tokenize)
{
	t_assign_context	ctx;
	t_token				*stack;
	t_paren				commands;

	stack = tokenize;
	commands = (t_paren)
	{false, false, '\0', false, 0, false, (t_quotes){false, false}};
	ctx = (t_assign_context){false, false, -1, NULL};
	initialize_stack(stack);
	while (stack && stack->value)
	{
		update_assignment_context(&ctx, stack);
		if (!parenthesis(stack, stack->value, &commands, &ctx))
			return (false);
		stack = stack->next;
	}
	if (!is_empty(stack))
		return (report_error(ERR_UNEXPECTED_TOKEN, ")"),
			free_stack(stack), false);
	free_stack(stack);
	return (true);
}

bool	check_count_paren(t_token *tokenize)
{
	int		paren_count;

	if (!tokenize)
		return (true);
	paren_count = count_parenthesis(tokenize);
	if (paren_count > 0)
	{
		report_error(ERR_UNEXPECTED_TOKEN, ")");
		return (false);
	}
	if (paren_count < 0)
	{
		report_error(ERR_UNEXPECTED_TOKEN, "(");
		return (false);
	}
	return (parenthesis_utils(tokenize));
}

void	update_assignment_context(t_assign_context *ctx, t_token *current)
{
	if (!current || !current->value)
		return ;
	ctx->equal_pos = -1;
	if (current->type == TOKEN_WORD)
	{
		if (is_assignment_command(current->value))
		{
			ctx->in_assignment = true;
			ctx->after_equals = false;
			ctx->assign_start = current;
		}
		else if (contains_assignment(current->value, &ctx->equal_pos))
		{
			ctx->in_assignment = true;
			ctx->after_equals = true;
			if (!ctx->assign_start)
				ctx->assign_start = current;
		}
		else if (ctx->in_assignment && ctx->assign_start
			&& is_assignment_command(ctx->assign_start->value))
			ctx->after_equals = true;
	}
	else if (ctx->in_assignment && (current->type == TOKEN_PIPE
			|| current->type == TOKEN_AND || current->type == TOKEN_OR))
		reset_assign(ctx);
}

bool	validation_parenthesis(t_token *tokenize)
{
	t_token				*current;
	t_token				*prev;
	t_assign_context	ctx;

	current = tokenize;
	prev = NULL;
	ctx = (t_assign_context){false, false, -1, NULL};
	if (current && current->value[0] == '(' && current->next
            && current->next->value[0] == '(')
    {
            if (!is_arithmetic_expression(current))
            {
                    report_error(ERR_SYNTAX,
                            "double parenthesis '((' not supported");
                    return (false);
		}
	}
	while (current)
	{
		update_assignment_context(&ctx, current);
		if (!check_command_paren_sequence(current, prev, &ctx))
			return (false);
		if (!check_paren_syntax(current, prev, &ctx))
			return (false);
		if (current->type != TOKEN_WHITESPACE)
			prev = current;
		current = current->next;
	}
	return (check_count_paren(tokenize));
}
