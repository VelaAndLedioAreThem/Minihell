/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/* +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:11:54 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/24 00:11:49 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

pid_t	g_child_pid = 0;

void	handle_input(char *input, t_env *env_list, int argc, char **argv)
{
	t_token			*tokens;
	char			*expandable;
	t_args			arg;
	t_ast			*ast;
	t_env			*env_copy;

	arg.exit_status = 0;
	if (*input)
	{
		add_history(input);
		arg.argc = argc - 1;
		arg.argv = argv;
		env_copy = deep_copy_env_list(env_list);
		if (!env_copy)
			return ;
		expandable = parse_env(input, env_copy, &arg);
		if (!expandable)
		{
			free_env_list(env_copy);
			return ;
		}
		printf("Tokenizing value\n");
		tokens = tokenize(expandable);
		if (!tokens)
		{
			free_tokens(tokens);
			return ;
		}
		if (!validation(tokens))
			return ;
		free(expandable);
		free_env_list(env_copy);
		ast = parse_tokens(tokens);
		free_tokens(tokens);
		if (!ast)
		{
			free_ast(ast);
			return ;
		}
		else if (ast)
			ast->env_list = env_list;
        execute_tree(ast, ast);
        free_ast(ast);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_env			*env_list;
	char *str;
    char *prompt;

	env_list = init_env_list(envp);
	handle_signal();
	while (1)
	{
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            str = ft_strjoin("minihell:",cwd);
            prompt = ft_strjoin(str,"$ ");
        }
        else
            prompt = ft_strdup("minishell$ "); // Fallback
        
		input = readline(prompt);
		free(prompt); // Free the generated prompt

		if (!input)
		{
			printf("exit\n");
			exit(0);
		}
		handle_input(input, env_list, argc, argv);
	}
	free_env_list(env_list);
}
