/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_paren_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:49:59 by ldurmish          #+#    #+#             */
/*   Updated: 2025/05/02 17:20:29 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

char	peek(t_token *stack)
{
	char		current;

	current = stack->top->name;
	if (is_empty(stack))
		return ('\0');
	return (current);
}

bool	if_there_is_filename(char *input, int *j, t_token *token)
{
	int			k;
	t_token		*next;

	if (!input[*j] || !is_valid_command_char(input[*j]))
	{
		if (!input[*j] && token->next)
		{
			next = token->next;
			k = 0;
			while (input[k] && ft_isspace(input[k]))
				k++;
			if (!next->value[k] || !is_valid_command_char(input[k]))
			{
				report_error(ERR_SYNTAX, "redirect without filename");
				return (free_stack(token), false);
			}
		}
	}
	return (true);
}

bool	it_is_redirect(char *input, int *i, int *j, t_token *token)
{
	if (ft_is_redirection(input[*i + 1]))
	{
		*j = *i + 1;
		if (input[*j + 1] && input[*j] == input[*j + 1])
			*j += 2;
		else
			*j += 1;
		while (input[*j] && ft_isspace(input[*j]))
			(*j)++;
		if (!if_there_is_filename(input, j, token))
			return (false);
		else
		{
			report_error(ERR_SYNTAX, "redirection without filename");
			return (free_stack(token), false);
		}
	}
	return (true);
}

bool	it_is_operator(char *input, int *i, int *j, t_token *token)
{
	if (input[*i + 1] == '&' || input[*i + 1] == '|')
	{
		if (!input[*i + 2])
		{
			if (!it_is_log_or_pipe(input, i, token))
				return (false);
		}
		else if (input[*i + 2] == input[*i + 3])
		{
			if (!it_is_logical_op(input, i, j, token))
				return (false);
		}
		else if (input[*i + 1] && input[*i + 2] != '|')
		{
			if (!it_is_pipe(input, i, j, token))
				return (false);
		}
	}
	return (true);
}

bool	check_after_close_paren(char *input, int *i, t_token *token)
{
	int			j;

	j = 0;
	if (!it_is_redirect(input, i, &j, token))
		return (false);
	else if (!it_is_operator(input, i, &j, token))
		return (false);
	return (true);
}
