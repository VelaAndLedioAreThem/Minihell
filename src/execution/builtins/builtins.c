#include "minishell.h"

typedef int (*t_builtin_func)(t_ast *data, t_ast *tree, int fd_out);

typedef struct s_builtin {
    const char      *name;
    t_builtin_func  func;
} t_builtin;

static int builtin_export(t_ast *data, t_ast *tree, int fd_out)
{
    if (!tree->cmd->args[1])
        print_env_list(data->env_list, fd_out); // Pass 1 to filter
    else
        execute_export(data, tree, fd_out);
    return 1;
}


static int builtin_unset(t_ast *data, t_ast *tree, int fd_out)
{
    (void)fd_out;
    if (execute_unset(data, tree))
        return (data->exit_status = 1, 1);
    return 1;
}

static int builtin_env(t_ast *data, t_ast *tree, int fd_out)
{
    (void)tree;
    print_env_list(data->env_list, fd_out); 
    return 1;
}

static int builtin_cd(t_ast *data, t_ast *tree, int fd_out)
{
    (void)fd_out; 
    char *path = tree->cmd->args[1];
    char *oldpwd = getcwd(NULL, 0); 

    if (!path || !ft_strcmp(path, "~"))
    {
        path = get_env_value(data->env_list, "HOME");
        if (!path)
        {
            ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
            free(oldpwd);
            return (data->exit_status = 1, 1);
        }
    }
    else if (!ft_strcmp(path, "-")) 
    {
        path = get_env_value(data->env_list, "OLDPWD");
        if (!path)
        {
            ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR_FILENO);
            free(oldpwd);
            return (data->exit_status = 1, 1);
        }
        ft_putendl_fd(path, STDOUT_FILENO); 
    }

    if (execute_cd(data, path))
    {
        free(oldpwd);
        return (data->exit_status = 1, 1);
    }

    set_env_var(data, "OLDPWD", oldpwd);
    free(oldpwd);
    return 1;
}

/* Builtin: pwd */
static int builtin_pwd(t_ast *data, t_ast *tree, int fd_out)
{
    (void)tree;
    if (execute_pwd(data, fd_out))
        return (data->exit_status = 1, 1);
    return 1;
}

/* Builtin: echo */
static int builtin_echo(t_ast *data, t_ast *tree, int fd_out)
{
    if (execute_echo(tree->cmd->args, fd_out))
        return (data->exit_status = 1, 1);
    return 1;
}

/* Builtin: exit */
static int builtin_exit(t_ast *data, t_ast *tree, int fd_out)
{
    (void)fd_out;
    if (execute_exit(data, tree))
        return (data->exit_status = 1, 1);
    return 1;
}

/* Builtin dispatch table */
static const t_builtin g_builtins[] = {
    { "export", builtin_export },
    { "unset",  builtin_unset  },
    { "env",    builtin_env    },
    { "cd",     builtin_cd     },
    { "pwd",    builtin_pwd    },
    { "echo",   builtin_echo   },
    { "exit",   builtin_exit   },
    { NULL,     NULL }
};

/*
 * handle_builtin - Check if the command is a builtin and execute it.
 * Returns 1 if a builtin was found and executed; otherwise, returns 0.
 */
int handle_builtin(t_ast *data, t_ast *tree, int fd_out)
{
    int i = 0;

    if (!tree->cmd->args || !tree->cmd->args[0])
        return 0;
    while (g_builtins[i].name)
    {
        if (!ft_strcmp(tree->cmd->args[0], g_builtins[i].name))
            return g_builtins[i].func(data, tree, fd_out);
        i++;
    }
    return 0;
}
