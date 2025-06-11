/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 22:22:31 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/11 08:39:11 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char *strip_quotes_and_parens(char *str)
{
    int len;
    int i;
    int paren_count = 0;
    int max_depth = 0;
    char *result;
    
    if (!str)
        return (NULL);
    
    len = ft_strlen(str);
    if (len == 0)
        return (ft_strdup(""));
    
    /* Validate parentheses - check for proper matching and no nesting */
    /* Skip whitespace and newlines when checking parentheses */
    i = 0;
    while (str[i])
    {
        if (str[i] == '(')
        {
            paren_count++;
            if (paren_count > max_depth)
                max_depth = paren_count;
        }
        else if (str[i] == ')')
        {
            paren_count--;
        }
        if (paren_count < 0)  /* More closing than opening */
            return (NULL);
        i++;
    }
    
    /* Check for unmatched parentheses or nested parentheses (depth > 1) */
    if (paren_count != 0 || max_depth > 1)
        return (NULL);
    
    /* If string is too short to have outer quotes/parens */
    if (len < 2)
        return (ft_strdup(str));
    
    /* Find first non-whitespace character */
    int start = 0;
    while (str[start] && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n'))
        start++;
    
    /* Find last non-whitespace character */
    int end = len - 1;
    while (end > start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n'))
        end--;
    
    /* Check for matching outer quotes or parentheses after trimming whitespace */
    if ((str[start] == '"' && str[end] == '"') || 
        (str[start] == '\'' && str[end] == '\'') || 
        (str[start] == '(' && str[end] == ')'))
    {
        /* Extract content between the outer quotes/parens, preserving inner whitespace */
        int content_len = end - start - 1;
        result = malloc(content_len + 1);
        if (!result)
            return (NULL);
        ft_strlcpy(result, str + start + 1, content_len + 1);
        return (result);
    }
    
    return (ft_strdup(str));
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
