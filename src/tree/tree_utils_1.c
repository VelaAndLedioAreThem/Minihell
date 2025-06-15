/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:45:55 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/16 00:45:53 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_commands	*create_command_struct(void)
{
	t_commands		*cmd;

	cmd = malloc(sizeof(t_commands));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->heredoc_delim = NULL;
	cmd->append = 0;
	cmd->redirections = NULL;
	cmd->is_builtin = 0;
	return (cmd);
}

t_ast	*init_subshell_node(t_token **tokens)
{
	t_token		*curr;
	t_ast		*node;

	if (!tokens || !*tokens)
		return (NULL);
	curr = *tokens;
	skip_tree_whitespaces(&curr);
	*tokens = curr;
	if (curr->type != TOKEN_PAREN_OPEN)
		return (NULL);
	node = create_ast_node(AST_SUBSHELL, curr);
	if (!node)
		return (NULL);
	*tokens = curr->next;
	skip_tree_whitespaces(tokens);
	return (node);
}

t_ast	*parse_subshell(t_token **tokens)
{
	t_ast		*node;
	t_token		*curr;

	node = init_subshell_node(tokens);
	if (!node)
		return (NULL);
	node->left = parse_command_line(tokens);
	if (!node->left)
	{
		free(node);
		return (NULL);
	}
	curr = *tokens;
	skip_tree_whitespaces(&curr);
	*tokens = curr;
	if (!curr || curr->type != TOKEN_PAREN_CLOSE)
	{
		free_ast(node);
		return (NULL);
	}
	*tokens = curr->next;
	return (node);
}

t_ast	*parse_command_words(t_token **tokens)
{
	t_ast		*cmd_node;
	t_token		*curr;
	t_token		*start;
	int			word_count;

	curr = *tokens;
	if (!skip_node(tokens, curr))
		return (NULL);
	start = curr;
	word_count = 0;
	while (curr && (curr->type == TOKEN_WORD || curr->type == TOKEN_WILDCARD))
	{
		word_count++;
		curr = curr->next;
		skip_tree_whitespaces(&curr);
	}
	if (word_count == 0)
		return (NULL);
	cmd_node = create_command_node(start, word_count);
	if (!cmd_node)
		return (NULL);
	*tokens = curr;
	return (cmd_node);
}

static int is_redirection_token(t_token_type type)
{
    return (type == TOKEN_APPEND || 
            type == TOKEN_REDIRECT_IN || 
            type == TOKEN_HEREDOC ||
            type == TOKEN_REDIRECT_OUT);
}


int set_command_name(t_ast *cmd_node, char *name)
{
    if (!cmd_node || !cmd_node->cmd || !name)
        return (0);
    
    // Free the empty args array
    if (cmd_node->cmd->args)
        free(cmd_node->cmd->args);
    
    // Allocate new args array with command name
    cmd_node->cmd->args = malloc(sizeof(char *) * 2);
    if (!cmd_node->cmd->args)
        return (0);
    
    cmd_node->cmd->args[0] = ft_strdup(name);
    cmd_node->cmd->args[1] = NULL;
    
    if (!cmd_node->cmd->args[0])
    {
        free(cmd_node->cmd->args);
        cmd_node->cmd->args = NULL;
        return (0);
    }
    
    return (1);
}

int add_command_arg(t_ast *cmd_node, char *arg)
{
    int argc = 0;
    char **new_args;
    
    if (!cmd_node || !cmd_node->cmd || !cmd_node->cmd->args || !arg)
        return (0);
    
    // Count current arguments
    while (cmd_node->cmd->args[argc])
        argc++;
    
    // Reallocate for new argument
    new_args = realloc(cmd_node->cmd->args, sizeof(char *) * (argc + 2));
    if (!new_args)
        return (0);
    
    cmd_node->cmd->args = new_args;
    cmd_node->cmd->args[argc] = ft_strdup(arg);
    cmd_node->cmd->args[argc + 1] = NULL;
    
    if (!cmd_node->cmd->args[argc])
        return (0);
    
    return (1);
}


t_ast *parse_simple_commands(t_token **tokens)
{
    t_ast *cmd_node = NULL;
    t_token *curr;
    int has_command = 0;
    
    if (!tokens || !*tokens)
        return (NULL);
    
    curr = *tokens;
    skip_tree_whitespaces(&curr);
    *tokens = curr;
    
    if (!curr)
        return (NULL);
    
    // Parse command words and redirections in any order
    while (curr)
    {
        skip_tree_whitespaces(&curr);
        *tokens = curr;
        
        if (!curr)
            break;
            
        if (is_redirection_token(curr->type))
        {
            // We need a command node to attach redirections to
            if (!cmd_node)
            {
                // Create empty command node for redirections
                cmd_node = create_ast_node(AST_COMMAND, curr);
                if (!cmd_node)
                    return (NULL);
                
                cmd_node->cmd = create_command_struct();
                if (!cmd_node->cmd)
                {
                    free(cmd_node);
                    return (NULL);
                }
                
                // Initialize empty args array
                cmd_node->cmd->args = malloc(sizeof(char *) * 1);
                if (!cmd_node->cmd->args)
                {
                    free(cmd_node->cmd);
                    free(cmd_node);
                    return (NULL);
                }
                cmd_node->cmd->args[0] = NULL;
            }
            
            // Parse redirection and add to command's redirection list
            if (!parse_redirection(tokens, cmd_node))
            {
                free_ast(cmd_node);
                return (NULL);
            }
            
            curr = *tokens;
        }
        else if (curr->type == TOKEN_WORD || curr->type == TOKEN_WILDCARD)
        {
            if (!has_command)
            {
                // Create initial command node or set command name
                if (!cmd_node)
                {
                    cmd_node = create_ast_node(AST_COMMAND, curr);
                    if (!cmd_node)
                        return (NULL);
                    
                    cmd_node->cmd = create_command_struct();
                    if (!cmd_node->cmd)
                    {
                        free(cmd_node);
                        return (NULL);
                    }
                }
                
                if (!set_command_name(cmd_node, curr->value))
                {
                    free_ast(cmd_node);
                    return (NULL);
                }
                
                has_command = 1;
            }
            else
            {
                // Add argument to command
                if (!add_command_arg(cmd_node, curr->value))
                {
                    free_ast(cmd_node);
                    return (NULL);
                }
            }
            curr = curr->next;
            *tokens = curr;
        }
        else
        {
            break;
        }
    }
    
    return (cmd_node);
}
