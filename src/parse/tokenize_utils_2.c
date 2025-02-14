/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:12:02 by ldurmish          #+#    #+#             */
/*   Updated: 2025/02/14 13:19:17 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int handle_word(t_token **token, char *input, int *i)
{
	int		start;
	char	*word;
	t_token	*current;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i])
		&& input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	word = ft_substr(input, start, *i - start);
	if (!word)
		return (-1);
	current = create_node(word, TOKEN_WORD);
	free(word);
	if (!current)
		return (-1);
	current->expandable = 1;
	append_node(token, current);
	(*i)--;
	return (1);
}
