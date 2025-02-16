/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/* +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:11:54 by ldurmish          #+#    #+#             */
/*   Updated: 2025/02/17 00:18:26 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_input(char *input, t_env *env_list, int argc, char **argv)
{
	t_token			*tokens;
	char			*expandable;
	t_args			arg;

	arg.exit_status = 0;
	if (*input)
	{
		add_history(input);
		arg.argc = argc - 1;
		arg.argv = argv;
		printf("The original output is : %s\n", input);
		expandable = parse_env(input, env_list, &arg);
		printf("After env expansion: '%s'\n", expandable);
		printf("Tokenizing value\n");
		tokens = tokenize(input);
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
		if (!input)
		{
			printf("exit\n");
			exit(0);
		}
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			exit(0);
		}
		handle_input(input, env_list, argc, argv);
	}
}
