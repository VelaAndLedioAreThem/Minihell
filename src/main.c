/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 21:52:54 by ldurmish          #+#    #+#             */
/*   Updated: 2025/04/12 23:20:13 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

pid_t	g_child_pid = 0;

int	main(int argc, char **argv, char **envp)
{
	t_env		*env_list;
	char		*input;
	char		*prompt;

	env_list = init_env_list(envp);
	handle_signal();
	incr_shell_lvl(env_list);
	while (1)
	{
		prompt = generate_prompt();
		input = readline(prompt);
		free(prompt);
		if (!input)
		{
			printf("exit\n");
			exit(1);
		}
		handle_input(input, env_list, argc, argv);
		free(input);
	}
	free_env_list(env_list);
	return (0);
}
