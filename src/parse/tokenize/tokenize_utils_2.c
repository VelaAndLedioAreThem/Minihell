/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:12:02 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/18 23:17:53 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	get_quote_content(char *input, int *i,
	char quote_type, char **content)
{
	int		start;
	int		len;

	start = *i + 1;
	*i = start;
	while (input[*i] && input[*i] != quote_type)
		(*i)++;
	if (!input[*i])
	{
		report_error(ERR_SYNTAX, "Unmatched parenthesis");
		return (-1);
	}
	len = *i - start;
	*content = ft_substr(input, start, len);
	if (!*content)
		return (-1);
	return (0);
}

int	handle_quotes(t_token **token, char *input, int *i)
{
	t_token	*current;
	char	*content;
	char	quote_type;

	quote_type = input[*i];
	if (get_quote_content(input, i, quote_type, &content) == -1)
		return (-1);
	current = create_node(content, TOKEN_WORD);
	if (!current)
	{
		free(content);
		return (-1);
	}
	if (quote_type == '"')
	{
		current->expandable = 1;
		current->double_quotes = 1;
	}
	else if (quote_type == '\'')
		current->single_quotes = 1;
	printf("Token: %s\n", current->value);
	append_node(token, current);
	return (1);
}

int	handle_word(t_token **token, char *input, int *i)
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
	printf("Token: %s\n", current->value);
	append_node(token, current);
	(*i)--;
	return (1);
}

int	handle_whitespace(t_token **token, char *input, int *i)
{
	int		start;
	char	*whitespace;
	t_token	*curr;

	start = *i;
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	whitespace = ft_substr(input, start, *i - start);
	if (!whitespace)
		return (-1);
	curr = create_node(whitespace, TOKEN_WHITESPACE);
	free(whitespace);
	if (!curr)
		return (-1);
	printf("TOKEN: Whitespace\n");
	append_node(token, curr);
	return (1);
}
