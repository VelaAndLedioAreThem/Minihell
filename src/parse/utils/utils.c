/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:58:49 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/30 17:25:04 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	ft_isspace(int num)
{
	if (num == ' ' || num == '\n' || num == '\t' || num == '\v' || num == '\f'
		|| num == '\r')
		return (1);
	return (0);
}

int	is_operator(char c)
{
	if (c == '&' || c == '|' || c == '(' || c == ')' || c == '>' || c == '<')
		return (1);
	return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	count_parenthesis_in_string(const char *str)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '(')
			count++;
		else if (str[i] == ')')
			count--;
		i++;
	}
	return (count);
}

int	count_parenthesis(t_token *tokenize)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokenize;
	while (current)
	{
		if (current->quotes.in_single_quotes
			|| current->quotes.in_double_quotes)
		{
			current = current->next;
			continue ;
		}
		if (current->type == TOKEN_PAREN_OPEN)
			count++;
		else if (current->type == TOKEN_PAREN_CLOSE)
			count--;
		else if (current->type == TOKEN_WORD && current->value)
			count += count_parenthesis_in_string(current->value);
		current = current->next;
	}
	return (count);
}
