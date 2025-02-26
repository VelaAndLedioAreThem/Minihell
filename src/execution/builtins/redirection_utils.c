#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * get_input_file - Traverses tree (assumed to be the left branch) for input redirection.
 * Opens the specified file for reading. Returns its file descriptor or -1 on error.
 * If no redirection is found, returns STDIN_FILENO.
 */
int get_input_file(t_tree *tree)
{
    t_tree *curr = tree->right;
    int fd = STDIN_FILENO;
    char *last_file = NULL;
    while (curr)
    {
        // Check for input redirection token (e.g., T_RED_INP)
        if (curr->type == T_RED_INP)
        {
            last_file = curr->value;
        }
        curr = curr->right;
    }
    if (last_file)
    {
        fd = open(last_file, O_RDONLY);
        if (fd < 0)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putendl_fd(last_file, STDERR_FILENO);
            return -1;
        }
    }
    return fd;    
}

int get_output_file(t_tree *tree)
{
    t_tree *curr = tree->right;
    int fd = STDOUT_FILENO;
    char *last_file = NULL;
    int redir_type = -1; // Track type during loop

    while (curr) {
        if (curr->type == T_RED_OUT || curr->type == T_APPEND) {
            last_file = curr->value;
            redir_type = curr->type; // Store type
        }
        curr = curr->right;
    }
    if (last_file) {
        int flags = O_WRONLY | O_CREAT;
        if (redir_type == T_RED_OUT) // Use tracked type
            flags |= O_TRUNC;
        else if (redir_type == T_APPEND)
            flags |= O_APPEND;
        fd = open(last_file, flags, 0644);
        if (fd < 0)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putendl_fd(last_file, STDERR_FILENO);
            return -1;
        }
    }
    return fd;
}
