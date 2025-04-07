
#include "../../../include/minishell.h"

static void sigint_handler(int sig) {
	(void)sig;
	if (g_child_pid == 0) {
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	} else {
		ft_putstr_fd("\n", STDOUT_FILENO);
		kill(g_child_pid, SIGINT);
	}
}
void	handle_signal(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
void	handle_sigint(int signo)
{
	if (signo == SIGINT)
	{
		if (isatty(STDIN_FILENO))
		{
			int i = write(1, "\n", 1);
			(void) i;
			if (g_child_pid == 42)
				g_child_pid = 44;
			if (g_child_pid != 0 && g_child_pid != 44)
			{
				kill(g_child_pid, SIGINT);
				g_child_pid++;
			}
			else
			{
				rl_on_new_line();
				rl_redisplay();
			}
		}
		else
			exit(EXIT_SUCCESS);
	}
}

void	handle_sigtstp_sigquit(int signo)
{
	if (signo == SIGTSTP || signo == SIGQUIT)
	{
		if (isatty(STDIN_FILENO))
		{
			if (g_child_pid == 0)
				rl_redisplay();
			else
			{
				kill(g_child_pid, signo);
				g_child_pid += 2;
			}
		}
	}
}

void	handle_c(int signo)
{
	handle_sigint(signo);
	handle_sigtstp_sigquit(signo);
}

int	handle_d(t_ast *data, char *line)
{
	(void)data;
	if (line == NULL)
	{
		rl_on_new_line();
		rl_redisplay();
	}
	if (ft_strlen(line) == 0)
	{
		ft_strdel(&line);
		return (1);
	}
	return (0);
}
