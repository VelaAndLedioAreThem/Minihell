/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_match_paren.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:18:18 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/12 15:48:55 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

static int	process_paren_char(char c, t_quotes *quote_type, int *paren_count)
{
	process_quotes(c, quote_type);
	if (!quote_type->in_double_quotes && !quote_type->in_single_quotes)
	{
		if (c == '(')
			(*paren_count)++;
		else if (c == ')')
			(*paren_count)--;
	}
	return (*paren_count);
}

static int	scan_token_paren(t_token *curr, char *input, int *total_pos,
	int start_pos)
{
	t_quotes	quote_type;
	int			paren_count;
	int			j;

	paren_count = 1;
	j = start_pos;
	quote_type = (t_quotes){false, false};
	while (curr)
	{
		input = curr->value;
		while (input[j] != '\0')
		{
			if (process_paren_char(input[j], &quote_type, &paren_count) == 0)
				return (*total_pos + j);
			j++;
		}
		*total_pos += ft_strlen(curr->value);
		j = 0;
		curr = curr->next;
	}
	return (-1);
}

int	find_matching_paren(t_token *token, char *input, int start_pos)
{
	t_token		*curr;
	int			total_pos;

	curr = token;
	total_pos = 0;
	while (curr && start_pos >= ft_strlen(curr->value))
	{
		start_pos -= ft_strlen(curr->value);
		curr = curr->next;
	}
	return (scan_token_paren(curr, input, &total_pos, start_pos));
}
