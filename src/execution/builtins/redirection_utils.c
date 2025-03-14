#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Updated get_input_file() in redirection_utils.c
int get_input_file(t_tree *tree) {
    t_tree *curr = tree; // Start at the current node
    int fd = STDIN_FILENO;
    char *last_file = NULL;

    // Traverse both left and right children recursively
    while (curr) {
        if (curr->type == T_RED_INP) {
            last_file = curr->value;
        }
        // Check left subtree for redirections
        if (curr->left) {
            int left_fd = get_input_file(curr->left);
            if (left_fd != STDIN_FILENO) {
                fd = left_fd;
            }
        }
        // Move to the right sibling
        curr = curr->right;
    }

    // Open the file if a redirection was found
    if (last_file) {
        fd = open(last_file, O_RDONLY);
        // In get_input_file() and get_output_file():
        if (fd < 0) {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(last_file, STDERR_FILENO);
            ft_putstr_fd(": ", STDERR_FILENO);
            ft_putendl_fd(strerror(errno), STDERR_FILENO); // Critical for debugging
            return -1;
        }
    }
    return fd;
}
int get_output_file(t_tree *tree)
{
    t_tree *curr = tree->left;
    int fd = STDOUT_FILENO;
    char *last_file = NULL;
    int redir_type = -1;

    while (curr) {
        if (curr->type == T_RED_OUT || curr->type == T_APPEND) {
            last_file = curr->value;
            redir_type = curr->type; 
        }
        curr = curr->left;
    }
    if (last_file) {
        int flags = O_WRONLY | O_CREAT;
        if (redir_type == T_RED_OUT) 
            flags |= O_TRUNC;
        else if (redir_type == T_APPEND)
            flags |= O_APPEND;
        fd = open(last_file, flags, 0644);
      // In get_input_file() and get_output_file():
        if (fd < 0) {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(last_file, STDERR_FILENO);
            ft_putstr_fd(": ", STDERR_FILENO);
            ft_putendl_fd(strerror(errno), STDERR_FILENO); // Critical for debugging
            return -1;
        }
    }
    return fd;
}