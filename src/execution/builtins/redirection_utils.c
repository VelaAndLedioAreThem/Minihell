#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int get_input_file(t_tree *tree, t_data *data)
{
    t_tree *node = tree->right;
    int fd = STDIN_FILENO;

    while (node)
    {
        if (node->type == T_RED_INP)
        {
            fd = open(node->value, O_RDONLY);
            if (fd < 0)
            {
                perror(node->value);
                return -1;
            }
        }
        else if (node->type == T_DELIM)
        {
            fd = handle_heredoc(node->value, data);
            if (fd < 0)
                return -1;
        }
        node = node->right;
    }
    return fd;
}

int get_output_file(t_tree *tree)
{
    t_tree *node = tree->right;
    int fd = STDOUT_FILENO;

    while (node)
    {
        if (node->type == T_RED_OUT || node->type == T_APPEND)
        {
            if (fd != STDOUT_FILENO)
                close(fd);
            if (node->type == T_RED_OUT)
                fd = open(node->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else
                fd = open(node->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror(node->value);
                return -1;
            }
        }
        node = node->right;
    }
    return fd;
}
