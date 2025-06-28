/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by codex             #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void merge_two_tokens(t_token *curr, t_token *next)
{
    char    *joined;

    joined = ft_strjoin(curr->value, next->value);
    if (!joined)
        return ;
    free(curr->value);
    curr->value = joined;
    if (next->expandable)
        curr->expandable = next->expandable;
    if (next->quotes.in_double_quotes)
        curr->quotes.in_double_quotes = true;
    if (next->quotes.in_single_quotes)
        curr->quotes.in_single_quotes = true;
    curr->next = next->next;
    free(next->value);
    free(next);
}

void    merge_adjacent_words(t_token *tokens)
{
    t_token *curr;

    curr = tokens;
    while (curr && curr->next)
    {
        if (curr->type == TOKEN_WORD && curr->next->type == TOKEN_WORD)
        {
            merge_two_tokens(curr, curr->next);
            continue ;
        }
        curr = curr->next;
    }
}
