/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:21:04 by ldurmish          #+#    #+#             */
/*   Updated: 2025/02/15 20:00:55 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	free_env_list(t_env *env_list)
{
	t_env		*temp;

	while (env_list)
	{
		temp = env_list->next;
		free(env_list->key);
		free(env_list->value);
		free(env_list);
		env_list = temp;
	}
}

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
