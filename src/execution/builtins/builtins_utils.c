#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/*
 * execute_pwd - Prints the current working directory to fd_out.
 * Returns 0 on success, 1 on error.
 */
int execute_pwd(t_data *data, int fd_out)
{
    (void)data; 

    char *cwd = getcwd(NULL, 0); 
    if (!cwd)
    {
        ft_putendl_fd("minishell: pwd: error retrieving current directory", STDERR_FILENO);
        return 1; 
    }

    ft_putendl_fd(cwd, fd_out);

    free(cwd);

    return 0;
}

/*
 * execute_echo - Prints the arguments to fd_out.
 * If "-n" is passed as the first argument, no trailing newline is printed.
 * Returns 0 on success.
 */
int execute_echo(char *args[], int fd_out)
{
    int i = 1;
    int newline = 1;

    if (args[1] && !ft_strcmp(args[1], "-n"))
    {
        newline = 0;
        i++;
    }
    while (args[i])
    {
        ft_putstr_fd(args[i], fd_out);
        if (args[i + 1])
            ft_putstr_fd(" ", fd_out);
        i++;
    }
    if (newline)
        ft_putendl_fd("", fd_out);
    return 0;
}

/*
 * execute_cd - Changes the current directory.
 * Uses path provided; if NULL, uses the HOME environment variable.
 * On failure, prints an error message and returns 1.
 * On success, updates data->curr_dir.
 */
int execute_cd(t_data *data, char *path)
{
    if (!path || chdir(path) != 0)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(strerror(errno), STDERR_FILENO);
        ft_putstr_fd("\n", STDERR_FILENO);
        return 1;
    }
    char *new_dir = get_root_directory();
    if (new_dir)
    {
        if (data->curr_dir)
            free(data->curr_dir);
        data->curr_dir = new_dir;
    }
    return 0;
}

/*
 * execute_export - Exports or updates an environment variable.
 * Expects tree->args_array[1] to be of the form "VAR=VALUE".
 */
#include <stdbool.h>

bool is_valid_identifier(const char *name) {
    if (name == NULL || *name == '\0')
        return false;
    
    char first = name[0];
    if (first != '_' && !((first >= 'A' && first <= 'Z') || (first >= 'a' && first <= 'z')))
        return false;
    
    for (int i = 1; name[i] != '\0'; i++) {
        char c = name[i];
        bool valid = (c == '_') || 
                    (c >= '0' && c <= '9') ||
                    (c >= 'A' && c <= 'Z') || 
                    (c >= 'a' && c <= 'z');
        if (!valid)
            return false;
    }
    return true;
}

t_envir *get_env_node(t_envir *env_list, const char *name) {
    t_envir *current = env_list;
    while (current != NULL) {
        if (strcmp(current->var_name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; // Not found
}

int execute_export(t_data *data, t_tree *tree, int fd_out)
{
    int i = 1; 
    int status = 0;
    
    while (tree->args_array[i]) {
        char *arg = tree->args_array[i];
        char *eq = ft_strchr(arg, '=');
        char *name = eq ? ft_substr(arg, 0, eq - arg) : ft_strdup(arg);

        if (!is_valid_identifier(name)) {
            ft_putstr_fd("minishell: export: ", STDERR_FILENO);
            ft_putstr_fd(arg, STDERR_FILENO);
            ft_putendl_fd(": not a valid identifier", STDERR_FILENO);
            status = 1;
            free(name);
            i++;
            continue;
        }

        t_envir *node = get_env_node(data->env_list, name);
        if (node) {
            if (eq) { // Only update value if '=' exists
                free(node->var_value);
                node->var_value = ft_strdup(eq + 1);
            }
            node->visible = 1; // Mark as exported
        } else
        {
        t_envir *new = malloc(sizeof(t_envir));
        if (!new)
        return (status = 1, status);
        new->var_name = name; // 'name' is already allocated above.
        new->var_value = eq ? ft_strdup(eq + 1) : ft_strdup("");
        new->visible = 1;
        new->next = data->env_list;
        new->prev = NULL;
        if (data->env_list)
        data->env_list->prev = new;
        data->env_list = new;
        }
        i++;
    }
    return status;
}

/*
 * execute_unset - Removes an environment variable.
 * tree->args_array[1] is the name of the variable to remove.
 */
int execute_unset(t_data *data, t_tree *tree)
{
    int i = 1;
    int status = 0;

    while (tree->args_array[i]) {
        char *name = tree->args_array[i];
        if (!is_valid_identifier(name)) {
            ft_putstr_fd("minishell: unset: ", STDERR_FILENO);
            ft_putstr_fd(name, STDERR_FILENO);
            ft_putendl_fd(": not a valid identifier", STDERR_FILENO);
            status = 1;
            i++;
            continue;
        }
        // Remove the variable from the environment list
        t_envir *current = data->env_list;
        while (current != NULL) {
            if (strcmp(current->var_name, name) == 0) {
                // Adjust pointers to remove the node
                if (current->prev != NULL)
                    current->prev->next = current->next;
                else
                    data->env_list = current->next; // Update head if current is the first node
                if (current->next != NULL)
                    current->next->prev = current->prev;
                // Free memory
                free(current->var_name);
                free(current->var_value);
                free(current);
                break; // Exit loop after removal
            }
            current = current->next;
        }
        i++;
    }
    return status;
}


/*
 * execute_exit - Exits the shell.
 * If an exit status is provided as an argument, it is used; otherwise, 0.
 * Frees allocated resources before exiting.
 */
int execute_exit(t_data *data, t_tree *tree)
{
    int exit_status = 0;
    if (tree->args_array[1])
        exit_status = ft_atoi(tree->args_array[1]);
    free_data(data);
    exit(exit_status);
    return 0;
}
