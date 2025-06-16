/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_wildcards_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:44:41 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/16 22:05:32 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_expanded_wildcard(char **args)
{
	t_expand_wild	exp;
	int				count;

	ft_memset(&exp, 0, sizeof(t_expand_wild));
	count = 0;
	exp.i = -1;
	while (args[++exp.i] != NULL)
	{
		if (has_wildcard(args[exp.i]))
		{
			exp.expanded = expand_wildcard(args[exp.i]);
			if (exp.expanded)
			{
				exp.j = -1;
				while (exp.expanded[++exp.j])
					count++;
				free_matches_array(exp.expanded);
			}
			else
				count++;
		}
		else
			count++;
	}
	return (count);
}

static void	fill_expand_args(char **temp_args, int temp_count,
	t_expand_wild *exp)
{
	exp->i = 0;
	exp->k = 0;
	while (exp->i < temp_count)
	{
		if (has_wildcard(temp_args[exp->i]))
		{
			exp->expanded = expand_wildcard(temp_args[exp->i]);
			if (exp->expanded)
			{
				exp->j = 0;
				while (exp->expanded[exp->j])
					exp->final_args[exp->k++]
						= ft_strdup(exp->expanded[exp->j++]);
				free_2darray(exp->expanded);
			}
			else
				exp->final_args[exp->k++] = ft_strdup(temp_args[exp->i]);
		}
		else
			exp->final_args[exp->k++] = ft_strdup(temp_args[exp->i]);
		exp->i++;
	}
	exp->final_args[exp->k] = NULL;
}

char	**expand_command_args(char **temp_args, int temp_count)
{
	t_expand_wild	exp;
	int				final_count;

	if (!temp_args)
		return (NULL);
	final_count = count_expanded_wildcard(temp_args);
	if (final_count <= 0)
		return (NULL);
	exp.final_args = malloc(sizeof(char *) * (final_count + 1));
	if (!exp.final_args)
		return (NULL);
	fill_expand_args(temp_args, temp_count, &exp);
	return (exp.final_args);
}

// This function determines if parentheses should be treated as a subshell
// or as part of command arguments
static int is_subshell_context(t_token *token)
{
    t_token *curr;
    
    if (!token || token->type != TOKEN_PAREN_OPEN)
        return (0);
    
    curr = token->next;
    skip_tree_whitespaces(&curr);
    
    // If we find command-like tokens after '(', it's likely a subshell
    while (curr && curr->type != TOKEN_PAREN_CLOSE)
    {
        if (curr->type == TOKEN_WORD)
        {
            // Look ahead to see if this looks like a command
            t_token *next = curr->next;
            skip_tree_whitespaces(&next);
            
            // If followed by pipe, redirect, logic operators, or more words, it's a command
            if (next && (next->type == TOKEN_PIPE || 
                        next->type == TOKEN_AND || 
                        next->type == TOKEN_OR ||
                        next->type == TOKEN_REDIRECT_OUT ||
                        next->type == TOKEN_REDIRECT_IN ||
                        next->type == TOKEN_APPEND ||
                        next->type == TOKEN_HEREDOC ||
                        next->type == TOKEN_WORD ||
                        next->type == TOKEN_PAREN_CLOSE))
                return (1);
            
            // Single word followed by close paren could be either
            if (next && next->type == TOKEN_PAREN_CLOSE)
                return (1);
        }
        else if (curr->type == TOKEN_PIPE || curr->type == TOKEN_AND || curr->type == TOKEN_OR)
        {
            return (1); // Definitely a subshell with operators
        }
        curr = curr->next;
    }
    
    return (0);
}

t_ast *parse_command(t_token **tokens)
{
    t_token *current;

    if (!tokens || !*tokens)
        return (NULL);
    
    current = *tokens;
    skip_tree_whitespaces(&current);
    *tokens = current;
    
    if (current->type == TOKEN_PAREN_OPEN)
    {
        if (is_subshell_context(current))
            return (parse_subshell(tokens));
        else
        {
            // This is parentheses as part of arguments, treat as simple command
            return (parse_simple_commands(tokens));
        }
    }
    
    return (parse_simple_commands(tokens));
}
