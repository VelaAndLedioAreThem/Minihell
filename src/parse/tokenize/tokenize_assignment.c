/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_assignment.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 00:19:26 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/23 17:59:09 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	is_assignment_pattern(char *input, int i)
{
	int	temp_i;

	temp_i = i;
	while (input[temp_i] && !ft_isspace(input[temp_i]))
	{
		if (input[temp_i] == '=')
			return (1);
		if (input[temp_i] == '|' || input[temp_i] == '&'
			|| input[temp_i] == '<' || input[temp_i] == '>')
			break ;
		temp_i++;
	}
	return (0);
}

static void	consume_quoted_string(char *input, int *i, char quote_char)
{
	(*i)++;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (input[*i] == quote_char)
		(*i)++;
}

static void	consume_assignment(char *input, int *i)
{
	while (input[*i] && !ft_isspace(input[*i]))
	{
		if (input[*i] == '|' || input[*i] == '&'
			|| input[*i] == '<' || input[*i] == '>')
			break ;
		if (input[*i] == '\'' || input[*i] == '"')
			consume_quoted_string(input, i, input[*i]);
		else
			(*i)++;
	}
}

static int	create_assignment(t_token **token, char *input, int start, int end)
{
	char		*assignment;
	t_token		*curr;

	assignment = ft_substr(input, start, end - start);
	if (!assignment)
		return (-1);
	curr = create_node(assignment, TOKEN_WORD);
	if (!curr)
		return (-1);
	curr->expandable = 1;
	append_node(token, curr);
	free(assignment);
	return (1);
}

int	handle_assignment(t_token **token, char *input, int *i)
{
	int		start;
	char	*assignment;

	start = *i;
	if (!is_assignment_pattern(input, *i))
		return (0);
	consume_assignment(input, i);
	assignment = ft_substr(input, start, *i - start);
	if (!assignment)
		return (-1);
	if (!validate_assignment_value(assignment))
	{
		free(assignment);
		report_error(ERR_SYNTAX, "invalid parenthesis in assignment");
		return (-1);
	}
	if (create_assignment(token, input, start, *i) == -1)
	{
		free(assignment);
		return (-1);
	}
	free(assignment);
	(*i)--;
	return (1);
}
