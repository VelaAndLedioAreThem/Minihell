/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 22:22:31 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/15 01:17:14 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	validate_parentheses(char *str)
{
	int		i;
	int		paren;

	i = 0;
	paren = 0;
	while (str[i])
	{
		if (str[i] == '(')
			paren++;
		else if (str[i] == ')')
			paren--;
		if (paren < 0 || paren > 1)
			return (0);
		i++;
	}
	if (paren != 0)
		return (0);
	return (1);
}

static int	should_strip_paren(char *str, int len)
{
	if (str[0] != '(' || str[len - 1] != ')')
		return (0);
	if (len > 1 && str[1] == '"' && str[len - 2] == '"')
		return (0);
	if (len > 1 && str[1] == '\'' && str[len - 2] == '\'')
		return (0);
	return (1);
}

static int	is_opening_quote(char c, t_quote_state *q)
{
	if (!q->in_quotes && (c == '"' || c == '\''))
	{
		q->in_quotes = 1;
		q->quote_char = c;
		return (1);
	}
	return (0);
}

static char	*strip_quotes(char *str, int start, int end)
{
	t_quote_state	quote;

	ft_memset(&quote, 0, sizeof(t_quote_state));
	quote.result = malloc(end - start + 1);
	if (!quote.result)
		return (NULL);
	quote.i = start;
	while (quote.i < end)
	{
		if (is_opening_quote(str[quote.i], &quote))
			continue ;
		else if (quote.in_quotes && str[quote.i] == quote.quote_char)
		{
			quote.in_quotes = 0;
			quote.quote_char = 0;
		}
		else
			quote.result[quote.j++] = str[quote.i];
		quote.i++;
	}
	quote.result[quote.j] = '\0';
	if (quote.in_quotes)
		return (free (quote.result), NULL);
	return (quote.result);
}

char	*remove_quotes_and_paren(char *str)
{
	int		len;
	int		start;
	int		end;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len == 0)
		return (ft_strdup(""));
	if (!validate_parentheses(str))
		return (NULL);
	start = 0;
	if (should_strip_paren(str, len))
		start = 1;
	if (start == 1)
		end = len - 1;
	else
		end = len;
	return (strip_quotes(str, start, end));
}
