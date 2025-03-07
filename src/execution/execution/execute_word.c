#include "minishell.h"

/*
 * Execute simple commands.
 * Uses get_input_file()/get_output_file() for redirection handling.
 * If not a built-in (via handle_builtin()), delegates to execute_command().
 */int execute_word(t_data *data, t_tree *tree)
{
    int saved_fds[2];
    
    save_std_fds(saved_fds);

    int fd_inp = get_input_file(tree, data);
    int fd_out = get_output_file(tree);

    if (fd_inp < 0 || fd_out < 0) {
        restore_std_fds(saved_fds);
        close_saved_fds(saved_fds);
        data->exit_status = 1;
        return data->exit_status;
    }

    dup2(fd_inp, STDIN_FILENO);
    dup2(fd_out, STDOUT_FILENO);

    if (handle_builtin(data, tree, STDOUT_FILENO))
        data->exit_status = data->exit_status;
    else
        data->exit_status = execute_command(data, tree, STDIN_FILENO, STDOUT_FILENO);

    safe_close_fd(&fd_inp);
    safe_close_fd(&fd_out);

    restore_std_fds(saved_fds); 
    close_saved_fds(saved_fds);

    return data->exit_status;
}
