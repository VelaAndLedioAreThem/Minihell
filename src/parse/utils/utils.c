/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:21:58 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/10 11:22:37 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	ft_isspace(int num)
{
	if (num == ' ' || num == '\n' || num == '\t'
		|| num == '\v' || num == '\f' || num == '\r')
		return (1);
	return (0);
}

int	is_operator(char c)
{
	if (c == '&' || c == '|' || c == '(' || c == ')'
		|| c == '>' || c == '<')
		return (1);
	return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int			i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int count_parenthesis(t_token *tokenize)
{
	int		count;
	t_token	*current;
	int		i;

	count = 0;
	current = tokenize;
	while (current)
	{
		// Skip quoted content - don't count parentheses inside quotes
		if (current->quotes.in_single_quotes || current->quotes.in_double_quotes)
		{
			current = current->next;
			continue;
		}
		
		// Only count parentheses in unquoted tokens
		if (current->type == TOKEN_PAREN_OPEN)
			count++;
		else if (current->type == TOKEN_PAREN_CLOSE)
			count--;
		else if (current->type == TOKEN_WORD && current->value)
		{
			// Count parentheses in unquoted word tokens
			i = 0;
			while (current->value[i])
			{
				if (current->value[i] == '(')
					count++;
				else if (current->value[i] == ')')
					count--;
				i++;
			}
		}
		current = current->next;
	}
	return (count);
}

