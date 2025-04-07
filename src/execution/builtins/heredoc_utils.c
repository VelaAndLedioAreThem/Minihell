#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>

static	void	handle_sigint(int sig, siginfo_t *info, void *context)
{
	(void)sig;
	(void)info;
	(void)context;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
}

static void	configure_signals(t_signal *sig)
{
	sig->sa_new.sa_sigaction = handle_sigint;
	sigemptyset(&sig->sa_new.sa_mask);
	sig->sa_new.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sig->sa_new, &sig->sa_old);
}

static int	process_input_loop(int fd, char *delimiter, t_signal *sig)
{
	char	*line;
	int		result;

	result = 0;
	while (!sig->sigint)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-doc delimited by EOF\n", 2);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		dprintf(fd, "%s\n", line);
		free(line);
	}
	if (sig->sigint)
		result = -1;
	return (result);
}

int	create_heredoc_temp_file(char *delimiter, t_ast *data)
{
	t_signal	sig;
	char		*tmpname;
	int			fd;

	tmpname = "/tmp/minishell_heredoc_XXXXXX";
	fd = mkstemp(tmpname);
	if (fd < 0)
		return (-1);
	configure_signals(&sig);
	data->heredoc_files = realloc(data->heredoc_files,
			sizeof(char *) * (data->heredoc_count + 1));
	data->heredoc_files[data->heredoc_count] = ft_strdup(tmpname);
	data->heredoc_count++;
	if (process_input_loop(fd, delimiter, &sig) == -1)
		return (-1);
	sigaction(SIGINT, &sig.sa_old, NULL);
	close(fd);
	return (open(tmpname, O_RDONLY));
}
