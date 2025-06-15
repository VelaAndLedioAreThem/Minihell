/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 22:06:24 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/16 00:44:24 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ast_type	get_redir_type(t_token *tokens)
{
	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_APPEND)
		return (AST_REDIR_APPEND);
	else if (tokens->type == TOKEN_HEREDOC)
		return (AST_REDIR_HERDOC);
	else if (tokens->type == TOKEN_REDIRECT_IN)
		return (AST_REDIR_IN);
	else if (tokens->type == TOKEN_REDIRECT_OUT)
		return (AST_REDIR_OUT);
	return (0);
}

t_ast	*create_redir(t_token **token, t_ast *cmd_node)
{
	t_token		*curr;
	t_ast		*node;

	curr = *token;
	if (!curr)
		return (NULL);
	node = create_ast_node(get_redir_type(curr), curr);
	if (!node)
		return (NULL);
	node->operator_type = curr->type;
	node->left = cmd_node;
	if (curr->next)
		curr = curr->next;
	else
		return (node);
	skip_tree_whitespaces(&curr);
	*token = curr;
	return (node);
}

t_redir_list *create_redir_node(int type, char *filename)
{
    t_redir_list *redir;
    
    redir = malloc(sizeof(t_redir_list));
    if (!redir)
        return (NULL);
    redir->type = type;
    redir->filename = ft_strdup(filename);
    if (!redir->filename)
    {
        free(redir);
        return (NULL);
    }
    redir->next = NULL;
    return (redir);
}

int add_redirection(t_commands *cmd, int type, char *filename)
{
    t_redir_list *new_redir;
    t_redir_list *curr;
    
    // Validate filename
    if (!filename || !*filename)
        return (0);
    
    new_redir = create_redir_node(type, filename);
    if (!new_redir)
        return (0);
    
    if (!cmd->redirections)
    {
        cmd->redirections = new_redir;
        return (1);
    }
    
    // Add to end of list to preserve order
    curr = cmd->redirections;
    while (curr->next)
        curr = curr->next;
    curr->next = new_redir;
    return (1);
}

static int is_redirection_token(t_token_type type)
{
    return (type == TOKEN_APPEND || 
            type == TOKEN_REDIRECT_IN || 
            type == TOKEN_HEREDOC ||
            type == TOKEN_REDIRECT_OUT);
}

static int validate_redirection_filename(char *filename)
{
    if (!filename || !*filename)
        return (0);
    
    // Check for invalid characters (basic validation)
    if (ft_strchr(filename, '/') && access(filename, F_OK) == -1)
    {
        // If it contains '/' but path doesn't exist, check parent directory
        char *dir = ft_strdup(filename);
        char *last_slash = ft_strrchr(dir, '/');
        if (last_slash)
        {
            *last_slash = '\0';
            if (access(dir, W_OK) == -1)
            {
                free(dir);
                return (0);
            }
        }
        free(dir);
    }
    
    return (1);
}

static int handle_single_redirection(t_token **tokens, t_commands *cmd)
{
    t_token *redir_token;
    t_token *filename_token;
    
    if (!tokens || !*tokens || !cmd)
        return (0);
    
    redir_token = *tokens;
    
    // Move to next token (should be filename)
    if (redir_token->next)
        *tokens = redir_token->next;
    else
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
        return (0);
    }
    
    skip_tree_whitespaces(tokens);
    
    // Check if we have a filename
    if (!*tokens || (*tokens)->type != TOKEN_WORD)
    {
        ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
        return (0);
    }
    
    filename_token = *tokens;
    
    // Validate filename
    if (!validate_redirection_filename(filename_token->value))
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(filename_token->value, 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        return (0);
    }
    
    // Add redirection to command's redirection list
    if (!add_redirection(cmd, redir_token->type, filename_token->value))
        return (0);
    
    // Move to next token
    *tokens = (*tokens)->next;
    return (1);
}

// Return int (success/failure) instead of t_ast*
int parse_redirection(t_token **tokens, t_ast *cmd_node)
{
    t_token *curr;
    
    if (!tokens || !*tokens || !cmd_node || !cmd_node->cmd)
        return (0);
    
    curr = *tokens;
    
    // Process all consecutive redirections
    while (curr && is_redirection_token(curr->type))
    {
        if (!handle_single_redirection(&curr, cmd_node->cmd))
            return (0);
        
        // Skip whitespace after filename
        skip_tree_whitespaces(&curr);
    }
    
    *tokens = curr;
    return (1); // Return success
}
