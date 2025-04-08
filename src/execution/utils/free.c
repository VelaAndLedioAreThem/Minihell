/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:54:56 by vszpiech          #+#    #+#             */
/*   Updated: 2025/04/08 14:45:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		free(token->value);
		free(token);
		token = tmp;
	}
}

void	free_heredoc_files(t_ast *data)
{
	int	i;

	if (!data->heredoc_files)
		return ;
	i = 0;
	while (i < data->heredoc_count)
	{
		if (data->heredoc_files[i])
		{
			unlink(data->heredoc_files[i]);
			free(data->heredoc_files[i]);
		}
		i++;
	}
	free(data->heredoc_files);
	data->heredoc_files = NULL;
	data->heredoc_count = 0;
}

void	free_data(t_ast *data)
{
	if (!data)
		return ;
	if (data->env_list)
		free_env_list(data->env_list);
	if (data->heredoc_files)
		free_heredoc_files(data);
	if (data)
		free_ast(data);
}

void	free_temp_data(t_ast *data)
{
	if (!data)
		return ;
	if (data->token)
		free_token(data->token);
	free(data);
}

void	ft_strdel(char **as)
{
	if (as && *as)
	{
		free(*as);
		*as = NULL;
	}
}
