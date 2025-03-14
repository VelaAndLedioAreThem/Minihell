#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>


char *get_env_value(t_envir *env_list, const char *key)
{
    while (env_list)
    {
        if (!ft_strcmp(env_list->var_name, key))
            return env_list->var_value;
        env_list = env_list->next;
    }
    return NULL;
}


void print_env_list(t_envir *env_list, int fd_out, int export_filter)
{
    while (env_list)
    {
        if (!export_filter || env_list->visible)
        {
            ft_putstr_fd(env_list->var_name, fd_out);
            ft_putstr_fd("=", fd_out);
            ft_putendl_fd(env_list->var_value ? env_list->var_value : "", fd_out);
        }
        env_list = env_list->next;
    }
}


char **env(t_envir **lst)
{
    t_envir *tmp = *lst;
    int count = 0;
    while (tmp) {
        if (tmp->visible) count++;
        tmp = tmp->next;
    }

    char **envp = malloc((count + 1) * sizeof(char *));
    tmp = *lst;
    int i = 0;
    while (tmp) {
        if (tmp->visible) {
            envp[i++] = ft_strjoin3(tmp->var_name, "=", tmp->var_value);
        }
        tmp = tmp->next;
    }
    envp[i] = NULL;
    return envp;
}

void incr_shell_lvl(t_data *data)
{
    char *shlvl_str = get_env_value(data->env_list, "SHLVL");
    int shlvl = 0;
    if (shlvl_str)
        shlvl = ft_atoi(shlvl_str);
    shlvl++;
    t_envir *tmp = data->env_list;
    while (tmp)
    {
        if (!ft_strcmp(tmp->var_name, "SHLVL"))
        {
            free(tmp->var_value);
            tmp->var_value = ft_itoa(shlvl);
            return;
        }
        tmp = tmp->next;
    }
    t_envir *new_env = malloc(sizeof(t_envir));
    if (new_env)
    {
        new_env->var_name = ft_strdup("SHLVL");
        new_env->var_value = ft_itoa(shlvl);
        new_env->visible = 1;
        new_env->next = data->env_list;
        new_env->prev = NULL;
        if (data->env_list)
            data->env_list->prev = new_env;
        data->env_list = new_env;
    }
}
char *ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
    char *temp = ft_strjoin(s1, s2);
    char *result = ft_strjoin(temp, s3);
    free(temp);
    return result;
}
void set_env_var(t_data *data, const char *var_name, const char *var_value)
{
    t_envir *env = data->env_list;
    while (env)
    {
        if (!ft_strcmp(env->var_name, var_name))
        {
            free(env->var_value);
            env->var_value = ft_strdup(var_value);
            return;
        }
        env = env->next;
    }
    t_envir *new_env = malloc(sizeof(t_envir));
    if (!new_env)
        return;
    new_env->var_name = ft_strdup(var_name);
    new_env->var_value = ft_strdup(var_value);
    new_env->visible = 1;
    new_env->next = data->env_list;
    new_env->prev = NULL;
    if (data->env_list)
        data->env_list->prev = new_env;
    data->env_list = new_env;
}
