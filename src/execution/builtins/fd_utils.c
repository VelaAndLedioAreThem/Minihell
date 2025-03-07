#include "minishell.h"
#include <unistd.h>

void save_std_fds(int saved_fds[2])
{
    saved_fds[0] = dup(STDIN_FILENO);
    saved_fds[1] = dup(STDOUT_FILENO);
}

void restore_std_fds(int saved_fds[2])
{
    dup2(saved_fds[0], STDIN_FILENO);
    dup2(saved_fds[1], STDOUT_FILENO);
}

void close_saved_fds(int saved_fds[2])
{
    close(saved_fds[0]);
    close(saved_fds[1]);
}

void safe_close_fd(int *fd)
{
    if (*fd > STDERR_FILENO)
    {
        close(*fd);
        *fd = -1;
    }
}
