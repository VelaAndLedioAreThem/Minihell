/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_parenthesis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 09:55:50 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/16 22:08:50 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	validate_paren_depth(t_token *curr, int *max_depth)
{
	int		paren_count;

	paren_count = 1;
	*max_depth = 1;
	while (curr && paren_count > 0)
	{
		if (curr->type == TOKEN_PAREN_OPEN)
		{
			paren_count++;
			if (paren_count > *max_depth)
				*max_depth = paren_count;
			if (*max_depth > 2)
				return (0);
		}
		else if (curr->type == TOKEN_PAREN_CLOSE)
			paren_count--;
		curr = curr->next;
	}
	return (paren_count == 0);
}

//static char	*join_extract_token(char *result, char *token_value)
//{
	//char	*tmp;
//
	//if (ft_strlen(result) > 0)
	//{
		//tmp = ft_strjoin(result, " ");
		//if (!tmp)
			//return (NULL);
		//result = tmp;
	//}
	//tmp = ft_strjoin(result, token_value);
	//free(result);
	//return (tmp);
//}
//
//static char	*extract_paren_content(t_token **curr)
//{
	//char	*result;
	//int		paren_count;
//
	//result = ft_strdup("");
	//if (!result)
		//return (NULL);
	//paren_count = 1;
	//while (*curr && paren_count > 0)
	//{
		//if ((*curr)->type == TOKEN_PAREN_OPEN)
			//paren_count++;
		//else if ((*curr)->type == TOKEN_PAREN_CLOSE)
		//{
			//paren_count--;
			//if (paren_count == 0)
				//break ;
		//}
		//if (paren_count > 0)
			//result = join_extract_token(result, (*curr)->value);
		//if (!result)
			//return (NULL);
		//*curr = (*curr)->next;
	//}
	//return (result);
//}
//
//static char	*combine_parentheses_tokens(t_token **token)
//{
	//char		*result;
	//int			max_depth;
//
	//if (!token || !*token || (*token)->type != TOKEN_PAREN_OPEN)
		//return (NULL);
	//if (!validate_paren_depth((*token)->next, &max_depth))
	//{
		//report_error(ERR_SYNTAX, "nested parentheses too deep");
		//return (NULL);
	//}
	//*token = (*token)->next;
	//result = extract_paren_content(token);
	//if (!result)
	//{
		//report_error(ERR_SYNTAX, "unmatched, parenthesis");
		//return (NULL);
	//}
	//if (*token)
		//*token = (*token)->next;
	//return (result);
//}

int handle_paren_token(t_expand_wild *exp, char ***temp_args)
{
    char *result;
    int max_depth;
    int paren_count;
    
    if (!exp->curr || exp->curr->type != TOKEN_PAREN_OPEN)
        return (0);
    
    // Validate parentheses depth
    if (!validate_paren_depth(exp->curr->next, &max_depth))
    {
        report_error(ERR_SYNTAX, "nested parentheses too deep");
        return (0);
    }
    
    // Extract content between parentheses
    result = ft_strdup("");
    if (!result)
        return (0);
    
    exp->curr = exp->curr->next; // Skip opening paren
    paren_count = 1;
    
    while (exp->curr && paren_count > 0)
    {
        if (exp->curr->type == TOKEN_PAREN_OPEN)
            paren_count++;
        else if (exp->curr->type == TOKEN_PAREN_CLOSE)
        {
            paren_count--;
            if (paren_count == 0)
                break;
        }
        
        if (paren_count > 0)
        {
            char *tmp;
            if (ft_strlen(result) > 0)
            {
                tmp = ft_strjoin(result, " ");
                free(result);
                if (!tmp)
                    return (0);
                result = tmp;
            }
            tmp = ft_strjoin(result, exp->curr->value);
            free(result);
            if (!tmp)
                return (0);
            result = tmp;
        }
        exp->curr = exp->curr->next;
    }
    
    if (paren_count != 0)
    {
        free(result);
        report_error(ERR_SYNTAX, "unmatched parenthesis");
        return (0);
    }
    
    // Move past closing paren
    if (exp->curr)
        exp->curr = exp->curr->next;
    
    (*temp_args)[exp->i] = result;
    exp->i++;
    return (1);
}

