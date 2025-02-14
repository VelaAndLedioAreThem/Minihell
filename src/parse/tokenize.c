/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:33:10 by ldurmish          #+#    #+#             */
/*   Updated: 2025/02/14 01:27:01 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_operator(t_token **token, char *input, int *i)
{
	int		status;

	if (input[*i] == '(' || input[*i] == ')')
		return (return_parenthesis(token, input[*i]));
	status = handle_double_operator(token, input, i);
	if (status != 0)
		return (status);
	return (handle_single_operator(token, input[*i]));
}

int	tokenize_utils(t_token **token, char *input, int *i)
{
	int		status;

	status = 0;
	if (is_operator(input[*i]))
		status = handle_operator(token, input, i);
	return (status);
}

t_token	*tokenize(char *input)
{
	t_token	*token;
	int		i;
	int		status;

	if (!input)
		return (NULL);
	i = 0;
	token = NULL;
	status = -1;
	if (input == NULL)
		return (NULL);
	while (input[i])
	{
		if (ft_isspace(input[i]))
		{
			i++;
			continue ;
		}
		status = tokenize_utils(&token, input, &i);
		i++;
	}
	return (token);
}
