
#include "../include/minishell.h"

t_ctx	*g_ctx;

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

	init_main_context(&g_main_ctx, argc, argv);
	env_list = init_env_list(envp);
	handle_signal(&g_main_ctx);
	incr_shell_lvl(env_list);
	while (1)
	{
		prompt = generate_prompt();
		input = readline(prompt);
		free(prompt);
		if (!input)
		{
			printf("exit\n");
			cleanup_minishell(env_list, NULL, NULL, NULL);
			signal(SIGQUIT, SIG_IGN);
			exit(get_last_exit_status(&g_main_ctx));
		}
		handle_input(input, env_list, &g_main_ctx);
		free(input);
	}
	return (cleanup_minishell(env_list, NULL, NULL, NULL), 0);
}
