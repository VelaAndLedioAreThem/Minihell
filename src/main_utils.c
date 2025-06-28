/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 23:19:06 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/28 20:05:17 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#define COL_BLUE "\033[0;34m"
#define COL_GREEN "\033[0;32m"
#define COL_RESET "\033[0m"

static int	is_valid_arith_expr(const char *expr)
{
	int	i;
	int	has_digit;

	if (!expr)
		return (0);
	i = 0;
	has_digit = 0;
	while (expr[i])
	{
		if (ft_isdigit(expr[i]))
			has_digit = 1;
		else if (ft_isspace(expr[i]) || ft_strchr("+-*/%()&|^~<>", expr[i]))
			;
		else
			return (0);
		i++;
	}
	return (has_digit);
}

static int	eval_double_parenthesis(const char *expr)
{
	char	cmd[1024];
	int		status;

	ft_bzero(cmd, sizeof(cmd));
	ft_strlcpy(cmd, "bash -c '(( ", sizeof(cmd));
	ft_strlcat(cmd, expr, sizeof(cmd));
	ft_strlcat(cmd, " ))' >/dev/null 2>&1", sizeof(cmd));
	status = system(cmd);
	if (status == -1)
		return (1);
	return (WEXITSTATUS(status));
}

static char	*preprocess_double_parenthesis(char *input)
{
	char		*start;
	char		*end;
	char		*expr;
	int			exit_st;
	const char	*replacement;
	char		*result;
	size_t		prefix_len;
	size_t		new_len;

	if (!input)
		return (NULL);
	start = ft_strnstr(input, "((", ft_strlen(input));
	if (!start)
		return (ft_strdup(input));
	if (start != input && *(start - 1) != '(' && !ft_isspace(*(start - 1)))
		return (ft_strdup(input));
	while (start[0] == '(' && start[1] == '(' && start[2] == '(')
		start++;
	end = ft_strnstr(start + 2, "))", ft_strlen(start + 2));
	if (!end)
		return (ft_strdup(input));
	expr = ft_substr(start + 2, 0, end - (start + 2));
	if (!expr)
		return (NULL);
	if (!is_valid_arith_expr(expr))
	{
		ft_putendl_fd("minishell: syntax error in arithmetic expression", 2);
		update_last_exit_status(g_ctx, 2);
		return (free(expr), NULL);
	}
	exit_st = eval_double_parenthesis(expr);
	free(expr);
	replacement = (exit_st == 0) ? "true" : "false";
	prefix_len = start - input;
	new_len = prefix_len + ft_strlen(replacement) + ft_strlen(end + 2) + 1;
	result = malloc(new_len);
	if (!result)
		return (NULL);
	ft_strlcpy(result, input, prefix_len + 1);
	ft_strlcat(result, replacement, new_len);
	ft_strlcat(result, end + 2, new_len);
	return (result);
}

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
	char	*processed;

	arg.exit_status = gles(ctx);
	if (*input)
	{
		add_history(input);
		processed = preprocess_double_parenthesis(input);
		if (!processed)
			return ;
               arg = (t_args){.argc = ctx->argc - 1, .argv = ctx->argv,
                               .exit_status = gles(ctx), .in_heredoc = 0};
		expandable = expand_and_tokenize(processed, env_list, &arg, &tokens);
		free(processed);
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
