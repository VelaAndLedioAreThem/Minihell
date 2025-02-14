/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 23:25:01 by ldurmish          #+#    #+#             */
/*   Updated: 2025/02/14 15:11:56 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_single_operator(t_token **token, char c)
{
	t_token		*current;

	if (c == '|')
		current = create_node("|", TOKEN_OR);
	else if (c == '<')
		current = create_node("<", TOKEN_REDIRECT_IN);
	else if (c == '>')
		current = create_node(">", TOKEN_REDIRECT_OUT);
	else if (c == '*')
		current = create_node("*", TOKEN_WILDCARD);
	else
		return (0);
	if (!current)
		return (-1);
	append_node(token, current);
	return (1);
}

static int	handle_double_operator_utils(t_token **current, char *input,
	int *i, char next_char)
{
	if (input[*i] == '<' && next_char == '<')
		*current = create_node("<<", TOKEN_HEREDOC);
	else if (input[*i] == '>' && next_char == '>')
		*current = create_node(">>", TOKEN_APPEND);
	else if (input[*i] == '&' && next_char == '&')
		*current = create_node("&&", TOKEN_AND);
	else if (input[*i] == '|' && next_char == '|')
		*current = create_node("||", TOKEN_OR);
	else
		return (0);
	if (!*current)
		return (-1);
	(*i)++;
	return (1);
}

int	handle_double_operator(t_token **head, char *input, int *i)
{
	t_token		*current;
	char		next_char;
	int			result;

	if (input[*i + 1] == '\0')
		return (0);
	next_char = input[*i + 1];
	result = handle_double_operator_utils(&current, input, i, next_char);
	if (result == 1)
		append_node(head, current);
	return (result);
}

int	return_parenthesis(t_token **token, char c)
{
	t_token		*current;

	if (c == '(')
		current = create_node("(", TOKEN_PAREN_OPEN);
	else
		current = create_node(")", TOKEN_PAREN_CLOSE);
	if (!current)
		return (-1);
	append_node(token, current);
	return (1);
}
