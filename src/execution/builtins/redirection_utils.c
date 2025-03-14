#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
// Updated get_input_file() in redirection_utils.c
int get_input_file(t_data *data, t_tree *tree) {
    t_tree *curr = tree;
    char *last_red_inp = NULL;
    char *last_heredoc_delim = NULL;
    int heredoc_fd = -1;
    int fd = STDIN_FILENO;

    while (curr) {
        if (curr->type == T_RED_INP) {
            last_red_inp = curr->value;
        } else if (curr->type == T_DELIM) {
            last_heredoc_delim = curr->value;
            heredoc_fd = create_heredoc_temp_file(last_heredoc_delim, data);
            if (heredoc_fd < 0) return -1;
            fd = heredoc_fd;
        }
        if (curr->left) {
            int left_fd = get_input_file(data, curr->left);
            if (left_fd != STDIN_FILENO) fd = left_fd;
        }
        curr = curr->right;
    }

    if (!last_heredoc_delim && last_red_inp) {
        fd = open(last_red_inp, O_RDONLY);
        if (fd < 0) {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(last_red_inp, STDERR_FILENO);
            ft_putstr_fd(": ", STDERR_FILENO);
            ft_putendl_fd(strerror(errno), STDERR_FILENO);
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