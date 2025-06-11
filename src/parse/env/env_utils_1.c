/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 22:22:31 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/11 07:58:45 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	validate_parentheses(char *str, int *total_paren)
{
	int		i;
	int		paren_count;
	int		max_depth;

	i = 0;
	paren_count = 0;
	max_depth = 0;
	*total_paren = 0;
	while (str[i])
	{
		if (str[i] == '(')
		{
			paren_count++;
			(*total_paren)++;
			if (paren_count > max_depth)
				max_depth = paren_count;
		}
		else if (str[i] == ')')
		{
			paren_count--;
			(*total_paren)++;
		}
		if (paren_count < 0)
			return (0);
		i++;
	}
	// Check for unmatched parentheses or nested parentheses (depth > 1)
	if (paren_count != 0 || max_depth > 1)
		return (0);
	return (1);
}

static int	is_quote_wrapped(char *str, int len)
{
	if ((str[0] == '"' && str[len - 1] == '"')
		|| (str[0] == '\'' && str[len - 1] == '\''))
		return (1);
	return (0);
}

static int	should_strip_outer_paren(char *str, int len)
{
	int			quote_wrapped;
	int			start_paren;
	int			end_paren;

	quote_wrapped = is_quote_wrapped(str, len);
	start_paren = 0;
	end_paren = 0;
	if (str[0] == '(')
		start_paren = 1;
	if (str[len - 1] == ')')
		end_paren = 1;
	if (!quote_wrapped && start_paren && end_paren)
		return (1);
	return (0);
}

static char	*remove_quotes(char *str, int len, int strip_paren)
{
	char	*result;
	int		i, j;
	int		in_quotes;
	char	quote_char;
	int		start_idx, end_idx;

	/* Determine start and end indices based on whether to strip parentheses */
	start_idx = strip_paren ? 1 : 0;
	end_idx = strip_paren ? len - 1 : len;
	
	result = malloc(end_idx - start_idx + 1);
	if (!result)
		return (NULL);
	
	i = start_idx;
	j = 0;
	in_quotes = 0;
	quote_char = 0;
	
	while (i < end_idx)
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			in_quotes = 1;
			quote_char = str[i];
			i++; /* Skip opening quote */
			continue;
		}
		else if (in_quotes && str[i] == quote_char)
		{
			in_quotes = 0;
			quote_char = 0;
			i++; /* Skip closing quote */
			continue;
		}
		
		result[j++] = str[i++];
	}
	
	result[j] = '\0';
	
	/* Check for unclosed quotes */
	if (in_quotes)
	{
		free(result);
		return (NULL);
	}
	
	return (result);
}

char	*remove_quotes_and_paren(char *str)
{
	int		total_paren;
	int		len;
	int		strip_paren;

	if (!str)
		return (NULL);
	
	len = ft_strlen(str);
	if (len == 0)
		return (ft_strdup(""));
	
	if (!validate_parentheses(str, &total_paren))
		return (NULL); /* Return NULL for invalid parentheses (including nested ones) */
	
	strip_paren = should_strip_outer_paren(str, len);
	
	return (remove_quotes(str, len, strip_paren));
}

char *strip_quotes_and_parens_tokens(t_token *tokens)
{
    t_token *current;
    int paren_count = 0;
    int max_depth = 0;
    char *result;
    int result_len = 0;
    t_token *start_content = NULL;
    t_token *end_content = NULL;
    int found_start_paren = 0;
    
    if (!tokens)
        return (NULL);
    
    /* First pass: validate parentheses and find content boundaries */
    current = tokens;
    while (current)
    {
        if (current->type == TOKEN_PAREN_OPEN|| ft_strcmp(current->value, "(") == 0)
        {
            paren_count++;
            if (paren_count > max_depth)
                max_depth = paren_count;
            if (!found_start_paren)
            {
                found_start_paren = 1;
                start_content = current->next;
            }
        }
        else if (current->type == TOKEN_PAREN_CLOSE || ft_strcmp(current->value, ")") == 0)
        {
            paren_count--;
            if (paren_count < 0)
                return (NULL); /* Unmatched closing */
            if (paren_count == 0)
                end_content = current;
        }
        current = current->next;
    }
    
    /* Check for unmatched or nested parentheses */
    if (paren_count != 0 || max_depth > 1)
        return (NULL);
    
    /* If no parentheses, handle quotes in single token or join all */
    if (!found_start_paren)
    {
        /* Check if single token with quotes */
        if (tokens && !tokens->next)
        {
            char *token_val = tokens->value;
            int len = ft_strlen(token_val);
            if (len >= 2 && 
                ((token_val[0] == '"' && token_val[len-1] == '"') ||
                 (token_val[0] == '\'' && token_val[len-1] == '\'')))
            {
                result = malloc(len - 1);
                if (!result)
                    return (NULL);
                ft_strlcpy(result, token_val + 1, len - 1);
                return (result);
            }
        }
        
        /* Join all tokens */
        current = tokens;
        while (current)
        {
            result_len += ft_strlen(current->value);
            if (current->next)
                result_len++; /* space */
            current = current->next;
        }
        
        result = malloc(result_len + 1);
        if (!result)
            return (NULL);
        result[0] = '\0';
        
        current = tokens;
        while (current)
        {
            ft_strcat(result, current->value);
            if (current->next)
                ft_strcat(result, " ");
            current = current->next;
        }
        return (result);
    }
    
    /* Extract content between parentheses */
    if (!start_content || start_content == end_content)
        return (ft_strdup(""));
    
    /* Calculate length needed */
    current = start_content;
    while (current && current != end_content)
    {
        result_len += ft_strlen(current->value);
        if (current->next && current->next != end_content)
            result_len++; /* space */
        current = current->next;
    }
    
    result = malloc(result_len + 1);
    if (!result)
        return (NULL);
    result[0] = '\0';
    
    /* Build result string */
    current = start_content;
    while (current && current != end_content)
    {
        ft_strcat(result, current->value);
        if (current->next && current->next != end_content)
            ft_strcat(result, " ");
        current = current->next;
    }
    
    return (result);
}
