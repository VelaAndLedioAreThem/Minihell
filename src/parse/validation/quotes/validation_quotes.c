/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_quotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 09:57:00 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/03 12:04:07 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

static bool	check_quotes(t_quotes *quote)
{
	if (quote->in_double_quotes || quote->in_single_quotes)
	{
		report_error(ERR_SYNTAX, "Unmtached %c quote detected");
		return (false);
	}
	return (true);
}

bool	validation_quotes(t_token *tokens)
{
	t_token		*curr;
	t_quotes	quote;
	int			i;
	char		*value;

	curr = tokens;
	quote = (t_quotes){false, false};
	while (curr)
	{
		if (curr->type == TOKEN_WORD)
		{
			value = curr->value;
			i = 0;
			while (value && value[i])
			{
				process_quotes(value[i], &quote);
				i++;
			}
		}
		curr = curr->next;
	}
	if (!check_quotes(&quote))
		return (false);
	return (true);
}
