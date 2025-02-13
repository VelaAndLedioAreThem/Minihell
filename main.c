/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:23:56 by vszpiech          #+#    #+#             */
/*   Updated: 2025/02/13 17:23:56 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "shell.h"

t_shell g_shell = {0};

int	main(void)
{
	setup_signals();
	printf("Starting minishell main loop\n");

	while (1)
	{
        if (g_shell.signal_received) {
            g_shell.last_exit_status = 130; // SIGINT exit code
            g_shell.signal_received = 0;
        }
		char cwd[128];
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("getcwd");
			// In case of error, you can use a default prompt
			strcpy(cwd, "unknown");
		}

		// Construct the prompt string.
		// Here,
		char prompt[1024];
		snprintf(prompt, sizeof(prompt), "minishell:~%s$ ", cwd);

		// Now use the dynamic prompt
		char *input = readline(prompt);
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		add_history(input);

		char **args = parse_command(input);
		if (!args || !args[0])
		{
			free(input);
			free_tokens(args);
			continue ;
		}
		// In main loop after getting input:
		if (strlen(input) > 0)
		{
			add_history(input);
		}
		else
		{
			free(input);
			continue ;
		}

		if (!handle_builtin(args))
		{
			pid_t pid = fork();
			if (pid == 0)
			{
				handle_redirections(args);
				execute_command(args);
				exit(EXIT_FAILURE);
			}
			else
			{
				wait(NULL);
			}
		}
		free(input);
		free_tokens(args);
	}
	return (0);
}
