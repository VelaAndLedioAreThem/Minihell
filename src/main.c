/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 18:49:31 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/28 18:49:31 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_ctx	*g_ctx;
static char *simple_gnl(int fd)
{
    char *line = NULL;
    char buf[1];
    size_t len = 0;
    ssize_t rd;
    while ((rd = read(fd, buf, 1)) > 0)
    {

        line = ft_realloc(line, len + 1, len + 2);
        if (!line)
            return NULL;
        line[len++] = buf[0];
        if (buf[0] == '\n')
            break;
    }
    if (rd <= 0 && len == 0)
        return NULL;
    line[len] = '\0';
    return line;
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
	return (cleanup_minishell(env_list, NULL, NULL, NULL),
		gles(&g_main_ctx));
}
