/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 23:25:01 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/28 19:20:37 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	handle_single_operator(t_token **token, char c)
{
	t_token		*current;

	if (c == '|')
		current = create_node("|", TOKEN_PIPE);
	else if (c == ' ')
		current = create_node(" ", TOKEN_WHITESPACE);
	else if (c == '<')
		current = create_node("<", TOKEN_REDIRECT_IN);
	else if (c == '>')
		current = create_node(">", TOKEN_REDIRECT_OUT);
	else if (c == '&')
	{
		report_error(ERR_SYNTAX, "background processes (&) are not supported");
		return (-1);
	}
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
	if ((input[*i] == '&' && next_char == '&' && input[*i + 2] == '&')
		|| (input[*i] == '|' && next_char == '|' && input[*i + 2] == '|'))
	{
		report_error(ERR_SYNTAX, "invalid operator sequence");
		return (-1);
	}
	result = handle_double_operator_utils(&current, input, i, next_char);
	if (result == 1)
	{
		append_node(head, current);
	}
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

static int	is_arithmetic_expr_valid(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i])
			&& str[i] != ' '
			&& str[i] != '+'
			&& str[i] != '-'
			&& str[i] != '*'
			&& str[i] != '/'
			&& str[i] != '%'
			&& str[i] != '('
			&& str[i] != ')')
			return (0);
		i++;
	}
	return (1);
}

int	validate_assignment_value(char *assignment)
{
	const char	*eq;
	const char	*value;
	const char	*end;
	char		*inner;
	size_t		len;

	eq = ft_strchr(assignment, '=');
	if (!eq || !*(eq + 1))
		return (1);
	value = eq + 1;
	if (ft_strncmp(value, "((", 2) != 0)
		return (1);
	len = ft_strlen(value);
	if (len < 4)
		return (0);
	end = value + len - 1;
	if (*(end) != ')' || *(end - 1) != ')')
		return (0);
	inner = ft_substr(value + 2, 0, len - 4);
	if (!inner)
		return (0);
	if (!is_arithmetic_expr_valid(inner))
	{
		free(inner);
		return (0);
	}
	free(inner);
	return (1);
}
