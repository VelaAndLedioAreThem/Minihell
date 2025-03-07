#include "minishell.h"
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

static volatile sig_atomic_t g_heredoc_interrupted = 0;

void handle_sigint_heredoc(int sig)
{
    (void)sig;
    g_heredoc_interrupted = 1;
    write(STDERR_FILENO, "\n", 1);
}

int handle_heredoc(const char *delimiter, t_data *data)
{
    char tmpfile[] = "/tmp/minishell_heredocXXXXXX";
    int fd = mkstemp(tmpfile);
    char *line;

    unlink(tmpfile);

    struct sigaction sa_old, sa_new;
    sa_new.sa_handler = handle_sigint_heredoc;
    sigemptyset(&sa_new.sa_mask);
    sa_new.sa_flags = 0;
    sigaction(SIGINT, &sa_new, &sa_old);

    g_heredoc_interrupted = 0;

    while (!g_heredoc_interrupted)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        char *expanded_line = expand_quotes(data, line);
        write(fd, expanded_line, ft_strlen(expanded_line));
        write(fd, "\n", 1);
        free(expanded_line);
        free(line);
    }

    sigaction(SIGINT, &sa_old, NULL);

    if (g_heredoc_interrupted)
    {
        close(fd);
        return -1;
    }

    lseek(fd, 0, SEEK_SET);
    return fd;
}
