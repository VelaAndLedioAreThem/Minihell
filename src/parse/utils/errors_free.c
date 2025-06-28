/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/02/14 13:21:04 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/22 16:44:55 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	free_env_list(t_env *env_list)
{
	t_env	*temp;

	while (env_list)
	{
		temp = env_list->next;
		free(env_list->key);
		free(env_list->value);
		free(env_list);
		env_list = temp;
	}
}

t_token	*free_tokens(t_token *token)
{
	t_token	*temp;

	while (token)
	{
		temp = token->next;
		if (token->value)
		{
			free(token->value);
			token->value = NULL;
		}
		if (token->top)
			free_stack(token);
		free(token);
		token = temp;
	}
	return (NULL);
}

void	cleanup_minishell(t_env *env_list, char *input, t_ast *ast_root,
		t_token *token)
{
	if (env_list)
		free_env_list(env_list);
	if (input)
		free(input);
	if (ast_root)
		free_ast(ast_root);
	if (token)
		free_tokens(token);
}

void	print_syntax_error(t_errors_code code, char *token)
{
	if (code == ERR_UNBALANCED_PAREN && token == NULL)
	{
		ft_putendl_fd("minishell: syntax error: unmatched parenthesis",
			STDERR_FILENO);
		return ;
	}
	ft_putstr_fd("minishell: syntax error near unexpected ", STDERR_FILENO);
	if (code == ERR_UNBALANCED_PAREN || code == ERR_UNEXPECTED_TOKEN)
		ft_putstr_fd("token `", STDERR_FILENO);
	else
		ft_putstr_fd("`", STDERR_FILENO);
	if (token)
		ft_putstr_fd(token, STDERR_FILENO);
	else
		ft_putstr_fd("newline", STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}

void	report_error(t_errors_code code, char *token)
{
	if (code == ERR_MEMORY)
	{
		ft_putendl_fd("minishell: memory allocation failed", STDERR_FILENO);
		return ;
	}
	if (code == ERR_PARSE)
	{
		ft_putendl_fd("minishell: parse error", STDERR_FILENO);
		return ;
	}
	print_syntax_error(code, token);
}
