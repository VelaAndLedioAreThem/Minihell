/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:21:04 by ldurmish          #+#    #+#             */
/*   Updated: 2025/02/14 13:26:16 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token	*free_tokens(t_token *token)
{
	t_token		*temp;

	while (token)
	{
		temp = (token)->next;
		free((token)->value);
		free(token);
		token = temp;
	}
	return (NULL);
}
