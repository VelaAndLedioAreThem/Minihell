/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:21:04 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/22 16:44:55 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	free_env_list(t_env *env_list)
{
	t_env		*temp;

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
	t_token		*temp;

	while (token)
	{
		temp = token->next;
		if (token->value)
		{
			free(token->value);
			token->value = NULL;
		}
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

void	report_error(t_errors_code code, char *token)
{
	if (code == ERR_UNBALANCED_PAREN)
	{
		if (token)
			printf("minishell: syntax error near unexpected token `%s'\n",
				token);
		else
			printf("minishell: syntax error: unmatched parenthesis\n");
	}
	else if (code == ERR_SYNTAX)
	{
		if (token)
			printf("minishell: syntax error near unexpected `%s'\n", token);
		else
			printf("minishell: syntax error\n");
	}
	else if (code == ERR_UNEXPECTED_TOKEN)
		printf("minishell: syntax error near unexpected token `%s'\n", token);
	else if (code == ERR_MEMORY)
		printf("minishell: memory allocation failed\n");
	else if (code == ERR_PARSE)
		printf("minishell: parse error\n");
}
