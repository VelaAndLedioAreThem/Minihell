/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_asterisk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eseferi <eseferi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:35:05 by eseferi           #+#    #+#             */
/*   Updated: 2024/04/11 13:16:32 by eseferi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_asterisk_token(t_token *token, t_data *data)
{
	char	*temp;
	int		i;

	i = 0;
	ft_strdel(&token->word);
	while (data->root_directory[i])
	{
		if (!token->word)
			token->word = ft_strjoin("", data->root_directory[i]);
		else
		{
			temp = ft_strjoin(token->word, data->root_directory[i]);
			ft_strdel(&token->word);
			token->word = temp;
		}
		temp = ft_strjoin(token->word, " ");
		ft_strdel(&token->word);
		token->word = temp;
		i++;
	}
}

void	extend_asterisk(t_token *token, t_data *data)
{
	if (is_only_asterisks(token->word))
		update_asterisk_token(token, data);
	else if (has_asterisk(token->word))
		check_matches(token, data->root_directory);
}

int match_pattern(const char *pattern, const char *text)
{
    if (!pattern || !text) return 0;
    
    while (*pattern)
    {
        if (*pattern == '*')
        {
            pattern++;
            while (*text && !match_pattern(pattern, text))
                text++;
        }
        else if (*pattern == '?' || *pattern == *text)
        {
            pattern++;
            text++;
        }
        else
        {
            return 0;
        }
    }
    return *text == '\0';
}

