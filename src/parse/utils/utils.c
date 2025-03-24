/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:21:58 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/24 18:39:34 by vszpiech         ###   ########.fr       */
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
		|| c == '>' || c == '<' || c == '*')
		return (1);
	return (0);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	len;
	size_t	i;

	len = 0;
	while (len < n && s[len])
		len++;
	dup = (char *)malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[len] = '\0';
	return (dup);
}
int	ft_strcmp(char *s1, const char *s2)
{
	int			i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	count_parenthesise_util(char *input)
{
	t_quotes	quotes;
	int			i;
	int			open_paren;
	int			close_paren;

	i = 0;
	open_paren = 0;
	close_paren = 0;
	quotes = (t_quotes){false, false};
	while (input[i] != '\0')
	{
		process_quotes(input[i], &quotes);
		if (!quotes.in_double_quotes && !quotes.in_single_quotes)
		{
			if (input[i] == '(')
				open_paren++;
			else if (input[i] == ')')
				close_paren++;
		}
		i++;
	}
	return (open_paren - close_paren);
}

int	count_parenthesis(t_token *tokens)
{
	int		count;
	int		total_count;

	if (!tokens)
		return (0);
	total_count = 0;
	while (tokens)
	{
		if (tokens->value)
		{
			count = count_parenthesise_util(tokens->value);
			total_count += count;
		}
		tokens = tokens->next;
	}
	return (total_count);
}

void ft_strdel(char **as)
{
    if (as && *as)
    {
        free(*as);
        *as = NULL;
    }
}
void free_2darray(char **array)
{
    int i;
    
    if (!array)
        return;
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}