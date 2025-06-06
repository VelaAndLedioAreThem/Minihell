#include "minishell.h"

void    update_last_exit_status(int status)
{
    g_last_exit_status = status;
}

int     get_last_exit_status(void)
{
    return (g_last_exit_status);
}
