/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:33:10 by ldurmish          #+#    #+#             */
/*   Updated: 2025/02/13 16:44:01 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*tokenize(char *input)
{
	t_token		*token;
	t_token		*current_token;
	int			i;

	token = NULL;
	current_token = 0;
	i = 0;
	if (input == NULL)
		return (NULL);
	while (input[i])
	{
		if ((input[i]))
			i++;
	}
}

