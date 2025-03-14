#include "minishell.h"


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


