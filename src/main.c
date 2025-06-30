/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 18:49:31 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/30 17:36:06 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_ctx		*g_ctx;

static char	*simple_gnl(int fd)
{
	char	*line;
	char	buffer[1];
	ssize_t	bytes_read;
	size_t	length;

	line = NULL;
	length = 0;
	bytes_read = read(fd, buffer, 1);
	while (bytes_read > 0)
	{
		line = ft_realloc(line, length + 1, length + 2);
		if (line == NULL)
			return (NULL);
		line[length] = buffer[0];
		length++;
		if (buffer[0] == '\n')
			break ;
		bytes_read = read(fd, buffer, 1);
	}
	if (bytes_read <= 0 && length == 0)
		return (NULL);
	line[length] = '\0';
	return (line);
}

static void	init_main_context(t_ctx *main_ctx, int argc, char **argv)
{
	g_ctx = main_ctx;
	main_ctx->child_pid = 0;
	main_ctx->last_exit_status = 0;
	main_ctx->argc = argc;
	main_ctx->argv = argv;
}

int	main(int argc, char **argv, char **envp)
{
	t_ctx	g_main_ctx;
	t_env	*env_list;
	char	*input;
	char	*prompt;
	char	*line;

	init_main_context(&g_main_ctx, argc, argv);
	env_list = init_env_list(envp);
	handle_signal(&g_main_ctx);
	incr_shell_lvl(env_list);
	while (1)
	{
		if (isatty(fileno(stdin)))
		{
			prompt = generate_prompt();
			input = readline(prompt);
			free(prompt);
		}
		else
		{
			line = simple_gnl(fileno(stdin));
			if (!line)
				break ;
			input = ft_strtrim(line, "\n");
			free(line);
		}
		if (!input)
		{
			cleanup_minishell(env_list, NULL, NULL, NULL);
			signal(SIGQUIT, SIG_IGN);
			exit(gles(&g_main_ctx));
		}
		handle_input(input, env_list, &g_main_ctx);
		free(input);
	}
	return (cleanup_minishell(env_list, NULL, NULL, NULL), gles(&g_main_ctx));
}
