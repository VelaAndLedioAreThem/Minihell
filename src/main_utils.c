/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 23:19:06 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/28 19:19:12 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#define COL_BLUE "\033[0;34m"
#define COL_GREEN "\033[0;32m"
#define COL_RESET "\033[0m"

static t_token	*tokenize_and_mark_expanded(char *input, char *expandable)
{
	t_token	*tokens;
	t_token	*curr;
	bool	was_expanded;

	tokens = tokenize(expandable);
	if (!tokens)
		return (NULL);
	was_expanded = (ft_strcmp(input, expandable) != 0);
	if (was_expanded)
	{
		curr = tokens;
		while (curr)
		{
			curr->from_expansion = 1;
			curr = curr->next;
		}
	}
	return (tokens);
}

static char	*expand_and_tokenize(char *input, t_env *env_list, t_args *arg,
		t_token **tokens)
{
	t_env	*copy;
	char	*expandable;

	copy = deep_copy_env_list(env_list);
	if (!copy)
		return (NULL);
	expandable = parse_env(input, copy, arg);
	if (!expandable)
	{
		free_env_list(copy);
		return (NULL);
	}
	*tokens = tokenize_and_mark_expanded(input, expandable);
	if (!*tokens)
	{
		free(expandable);
		free_env_list(copy);
		return (NULL);
	}
	free_env_list(copy);
	return (expandable);
}

static void	execute_input(t_token *t, t_env *env_list, char *exp, t_ctx *ctx)
{
	t_ast	*ast;

	ast = parse_tokens(t);
	free_tokens(t);
	if (!ast)
	{
		free(exp);
		update_last_exit_status(ctx, 2);
		return ;
	}
	ast->heredoc_files = NULL;
	ast->heredoc_count = 0;
	ast->env_list = env_list;
	execute_tree(ast, ast);
	update_last_exit_status(ctx, ast->exit_status);
	cleanup_heredoc_files(ast);
	free_ast(ast);
	free(exp);
}

void	handle_input(char *input, t_env *env_list, t_ctx *ctx)
{
	t_token	*tokens;
	t_args	arg;
	char	*expandable;

	arg.exit_status = gles(ctx);
	if (*input)
	{
		add_history(input);
		arg = (t_args){.argc = ctx->argc - 1, .argv = ctx->argv,
			.exit_status = gles(ctx)};
		expandable = expand_and_tokenize(input, env_list, &arg, &tokens);
		if (!expandable)
			return ;
		if (!validation(tokens))
		{
			free(expandable);
			update_last_exit_status(ctx, 2);
			return ;
		}
		execute_input(tokens, env_list, expandable, ctx);
	}
}

char	*generate_prompt(void)
{
	char	cwd[PATH_MAX];
	char	*tmp;
	char	*prompt;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		tmp = ft_strjoin(COL_BLUE "minishell:" COL_GREEN, cwd);
		prompt = ft_strjoin(tmp, COL_RESET "$ ");
		free(tmp);
		return (prompt);
	}
	return (ft_strdup("minishell$ "));
}
