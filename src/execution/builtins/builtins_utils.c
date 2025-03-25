/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/03/25 13:17:20 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/* Helper function to update environment variables */
/* Validation helper */
bool is_valid_identifier(const char *name)
{
    if (!name || !*name || ft_isdigit(*name))
        return false;
    while (*name)
    {
        if (!ft_isalnum(*name) && *name != '_')
            return false;
        name++;
    }
    return true;
}
/* Helper function to retrieve an environment variable node */
t_env *get_env_node(t_env *env_list, const char *name)
{
    t_env *current = env_list;
    while (current)
    {
        if (strcmp(current->key, name) == 0)
        return current;
        current = current->next;
    }
    return NULL;
}
static void update_env_var(t_ast *data, const char *key, const char *value)
{
    t_env *env = get_env_node(data->env_list, key);
    if (env)
    {
        free(env->value);
        env->value = ft_strdup(value);
    }
    else
    {
        t_env *new_env = malloc(sizeof(t_env));
        new_env->key = ft_strdup(key);
        new_env->value = ft_strdup(value);
        new_env->next = data->env_list;
        data->env_list = new_env;
    }
}

/* execute_pwd - Prints current working directory */
int execute_pwd(t_ast *data, int fd_out)
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

/* execute_echo - Prints arguments with option handling */
int execute_echo(char *args[], int fd_out)
{
    int i = 1;
    int N1 = 1;

    if (args[1] && !ft_strcmp(args[1], "-n"))
    {
        N1 = 0;
        i++;
    }
    while (args[i])
    {
        ft_putstr_fd(args[i], fd_out);
        if (args[i + 1])
            ft_putstr_fd(" ", fd_out);
        i++;
    }
    if (N1)
        ft_putendl_fd("", fd_out);
    return 0;
}

/* execute_cd - Changes directory and updates environment */
int execute_cd(t_ast *data, char *path)
{
    char *old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
    {
        ft_putendl_fd("minishell: cd: error retrieving current directory", STDERR_FILENO);
        return 1;
    }

    // Handle HOME if no path
    if (!path)
    {
        t_env *home = get_env_node(data->env_list, "HOME");
        if (!home || !home->value)
        {
            ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
            free(old_pwd);
            return 1;
        }
        path = home->value;
    }

    if (chdir(path) != 0)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(strerror(errno), STDERR_FILENO);
        ft_putstr_fd("\n", STDERR_FILENO);
        free(old_pwd);
        return 1;
    }

    // Update PWD and OLDPWD
    char *new_pwd = getcwd(NULL, 0);
    if (!new_pwd)
    {
        ft_putendl_fd("minishell: cd: error updating directory", STDERR_FILENO);
        free(old_pwd);
        return 1;
    }

    update_env_var(data, "OLDPWD", old_pwd);
    update_env_var(data, "PWD", new_pwd);
    
    free(old_pwd);
    free(new_pwd);
    return 0;
}

/* execute_export - Adds/updates environment variables */
int execute_export(t_ast *data, t_ast *tree, int fd_out)
{
    (void)fd_out;
    int status = 0;
    char *arg;

    for (int i = 1; tree->cmd->args[i]; i++)
    {
        arg = tree->cmd->args[i];
        char *eq = ft_strchr(arg, '=');
        char *name = eq ? ft_substr(arg, 0, eq - arg) : ft_strdup(arg);
        
        if (!name || !is_valid_identifier(name))
        {
            ft_putstr_fd("minishell: export: `", STDERR_FILENO);
            ft_putstr_fd(arg, STDERR_FILENO);
            ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
            free(name);
            status = 1;
            continue;
        }

        t_env *env = get_env_node(data->env_list, name);
        if (env)
        {
            if (eq)
            {
                free(env->value);
                env->value = ft_strdup(eq + 1);
            }
            free(name);
        }
        else
        {
            t_env *new_env = malloc(sizeof(t_env));
            new_env->key = name;
            new_env->value = eq ? ft_strdup(eq + 1) : ft_strdup("");
            new_env->next = data->env_list;
            data->env_list = new_env;
        }
    }
    return status;
}

/* execute_unset - Removes environment variables */
int execute_unset(t_ast *data, t_ast *tree)
{
    int status = 0;

    for (int i = 1; tree->cmd->args[i]; i++)
    {
        char *name = tree->cmd->args[i];
        if (!is_valid_identifier(name))
        {
            ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
            ft_putstr_fd(name, STDERR_FILENO);
            ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
            status = 1;
            continue;
        }

        t_env *prev = NULL;
        t_env *curr = data->env_list;
        while (curr)
        {
            if (!strcmp(curr->key, name))
            {
                if (prev)
                    prev->next = curr->next;
                else
                    data->env_list = curr->next;
                free(curr->key);
                free(curr->value);
                free(curr);
                break;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    return status;
}
/* execute_exit - Exits shell with status */
int execute_exit(t_ast *data, t_ast *tree)
{
    int status = 0;
    if (tree->cmd->args[1])
        status = ft_atoi(tree->cmd->args[1]);
    free_data(data);
    exit(status);
    return 0;
}

