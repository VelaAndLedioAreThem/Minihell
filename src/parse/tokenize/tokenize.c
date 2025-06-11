/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:33:10 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/10 10:57:14 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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

	status = handle_assignment(token, input, i);
	if (status != 0)
		return (status);
	if (input[*i] == '\'' || input[*i] == '"')
		status = handle_quotes(token, input, i);
	else if (is_operator(input[*i]))
	{
		status = handle_operator(token, input, i);
		if (status == 0)
			status = handle_word(token, input, i);
	}
	else
		status = handle_word(token, input, i);
	return (status);
}

t_token	*finalize_token(t_token *token)
{
	t_token	*eof_token;

	eof_token = create_node("", TOKEN_EOF);
	if (!eof_token)
		return (free_tokens(token));
	append_node(&token, eof_token);
	return (token);
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
			status = handle_whitespace(&token, input, &i);
			continue ;
		}
		status = tokenize_utils(&token, input, &i);
		if (status == -1)
			return (free_tokens(token));
		i++;
	}
	return (finalize_token(token));
}
