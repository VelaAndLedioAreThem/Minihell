/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:20:22 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/30 17:23:17 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	merge_with_next_word(t_token *curr)
{
	t_token	*next;
	char	*joined;

	next = curr->next;
	joined = ft_strjoin(curr->value, next->value);
	if (!joined)
		return ;
	free(curr->value);
	curr->value = joined;
	curr->expandable = curr->expandable || next->expandable;
	curr->quotes.in_single_quotes = curr->quotes.in_single_quotes
		&& next->quotes.in_single_quotes;
	curr->quotes.in_double_quotes = curr->quotes.in_double_quotes
		&& next->quotes.in_double_quotes;
	curr->next = next->next;
	free(next->value);
	if (next->top)
		free_stack(next);
	free(next);
}

void	merge_word_tokens(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	while (curr && curr->next)
	{
		if (curr->type == TOKEN_WORD && curr->next->type == TOKEN_WORD)
		{
			merge_with_next_word(curr);
			continue ;
		}
		curr = curr->next;
	}
}
