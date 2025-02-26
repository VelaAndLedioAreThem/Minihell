#include "minishell.h"

/*
 * Execute AND (&&) operations:
 * Run the right branch only if the left branch returns 0 (success).
 */
int execute_and(t_data *data, t_tree *tree)
{
    int left_status = execute(data, tree->left);
    if (left_status != 0)
    {
        data->exit_status = left_status;
        return left_status;
    }
    int right_status = execute(data, tree->right);
    data->exit_status = right_status;
    return right_status;
}

/*
 * Execute OR (||) operations:
 * Run the right branch only if the left branch returns nonzero (failure).
 */
int execute_or(t_data *data, t_tree *tree)
{
    int left_status = execute(data, tree->left);
    if (left_status == 0)
    {
        data->exit_status = 0;
        return 0;
    }
    int right_status = execute(data, tree->right);
    data->exit_status = right_status;
    return right_status;
}

/* 
 * The redirection helper functions (get_input_file() and get_output_file())
 * remain unchanged; they open file descriptors and report errors as needed.
 */
