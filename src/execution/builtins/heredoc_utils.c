#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>

static int g_heredoc_sigint = 0;

static void sigint_handler(int sig) {
    (void)sig;
    g_heredoc_sigint = 1;
    ioctl(STDIN_FILENO, TIOCSTI, "\n");
}

int create_heredoc_temp_file(char *delimiter, t_ast *data) {
    int fd;
    char *line;
    char tmpname[] = "/tmp/minishell_heredoc_XXXXXX";

    fd = mkstemp(tmpname);
    if (fd < 0) return -1;

    // Add to heredoc_files for cleanup
    data->heredoc_files = realloc(data->heredoc_files, sizeof(char*) * (data->heredoc_count + 1));
    data->heredoc_files[data->heredoc_count] = ft_strdup(tmpname);
    data->heredoc_count++;

    // Set up SIGINT handler
    g_heredoc_sigint = 0;

    // Set up signal handler
    struct sigaction sa_new, sa_old;
    sa_new.sa_handler = sigint_handler;
    sigemptyset(&sa_new.sa_mask);
    sa_new.sa_flags = 0;
    sigaction(SIGINT, &sa_new, &sa_old);

    while (!g_heredoc_sigint) {
        line = readline("> ");
        if (!line) {
            ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n", STDERR_FILENO);
            break;
        }
        if (ft_strcmp(line, delimiter) == 0) {
            free(line);
            break;
        }
        dprintf(fd, "%s\n", line);
        free(line);
    }

    // Restore original signal handler
    sigaction(SIGINT, &sa_old, NULL);
    close(fd);

    // Re-open for reading
    fd = open(tmpname, O_RDONLY);
    return (g_heredoc_sigint) ? -1 : fd;
}