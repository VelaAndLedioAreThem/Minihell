#include "minishell.h"

/*
 * Execute simple commands.
 * Uses get_input_file()/get_output_file() for redirection handling.
 * If not a built-in (via handle_builtin()), delegates to execute_command().
 */
int execute_word(t_data *data, t_tree *tree)
{
    int fd_in;
    int fd_out;

    fd_in = get_input_file(tree);    /* Opens input redirection (or returns STDIN_FILENO) */
    fd_out = get_output_file(tree);    /* Opens output redirection (or returns STDOUT_FILENO) */
    
    if (fd_in < 0 || fd_out < 0)
    {
        data->exit_status = 1;
        return 1;
    }
    
    /* Builtin command check */
    if (handle_builtin(data, tree, fd_out))
    {
        if (fd_in != STDIN_FILENO)
            close(fd_in);
        if (fd_out != STDOUT_FILENO)
            close(fd_out);
        return data->exit_status;
    }
    
    /* External command execution */
    data->exit_status = execute_command(data, tree, fd_in, fd_out);
    if (fd_in != STDIN_FILENO)
        close(fd_in);
    if (fd_out != STDOUT_FILENO)
        close(fd_out);
    return data->exit_status;
}
