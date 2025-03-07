/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/* +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:11:54 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/07 19:01:18 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_input(char *input, t_env *env_list, int argc, char **argv)
{
	t_token			*tokens;
	//char			*expandable;
//	t_args			arg;
	
	(void)argc;
	(void)argv;
	(void)env_list;
	//arg.exit_status = 0;
	if (*input)
	{
		add_history(input);
		/*
		arg.argc = argc - 1;
		arg.argv = argv;*/
		printf("The original output is : %s\n", input);
		//expandable = parse_env(input, env_list, &arg);
		printf("Tokenizing value\n");
		tokens = tokenize(input);
		if (!tokens)
		{
			free_tokens(tokens);
			return ;
		}
		if (!validation(tokens))
			return ;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_env			*env_list;

	env_list = init_env_list(envp);
	while (1)
	{
		input = readline("minishell$: ");
		if (!input) {
			printf("exit\n");
			exit(0);
		}
		handle_input(input, env_list, argc, argv);
	}
}
