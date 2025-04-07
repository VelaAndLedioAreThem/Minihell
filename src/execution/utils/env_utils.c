#include "../../include/minishell.h"
#include <stdlib.h>
#include <unistd.h>

char *ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
    char *tmp = ft_strjoin(s1, s2);
    char *res = ft_strjoin(tmp, s3);
    free(tmp);
    return res;
}


void print_env_list(t_env *env_list, int fd_out)
{
    while (env_list)
    {
        ft_putstr_fd(env_list->key, fd_out);      
        ft_putstr_fd("=", fd_out);
        ft_putendl_fd(env_list->value ? env_list->value : "", fd_out);
        env_list = env_list->next;
    }
}

char **env(t_env **lst)
{
    t_env *tmp = *lst;
    int count = 0;
    
    while (tmp) {
        count++;
        tmp = tmp->next;
    }

    char **envp = malloc((count + 1) * sizeof(char *));
    tmp = *lst;
    int i = 0;
    while (tmp) {
        envp[i++] = ft_strjoin3(tmp->key, "=", tmp->value);
        tmp = tmp->next;
    }
    envp[i] = NULL;
    return envp;
}

void incr_shell_lvl(t_ast *data)
{
    char *shlvl_str = get_env_value(data->env_list, "SHLVL");
    int shlvl = 0;
    
    if (shlvl_str)
        shlvl = ft_atoi(shlvl_str);
    shlvl++;

    t_env *tmp = data->env_list;
    while (tmp)
    {
        if (!ft_strcmp(tmp->key, "SHLVL"))
        {
            free(tmp->value);
            tmp->value = ft_itoa(shlvl);
            return;
        }
        tmp = tmp->next;
    }
    t_env *new_env = malloc(sizeof(t_env));
    if (new_env)
    {
        new_env->key = ft_strdup("SHLVL");
        new_env->value = ft_itoa(shlvl);
        new_env->next = data->env_list;
        data->env_list = new_env;
    }
}
void set_env_var(t_ast *data, char *var_name, const char *var_value)
{
    t_env *env = data->env_list;
    while (env)
    {
        if (!ft_strcmp(env->key, var_name))
        {
            free(env->value);
            env->value = ft_strdup(var_value);
            return;
        }
        env = env->next;
    }
    t_env *new_env = malloc(sizeof(t_env));
    if (!new_env)
        return;
    new_env->key = ft_strdup(var_name);
    new_env->value = ft_strdup(var_value);
    new_env->next = data->env_list;
    data->env_list = new_env;
}

void	free_paths(char **paths, char **original_paths)
{
	paths = original_paths;
	while (*paths)
	{
		ft_strdel(paths);
		paths++;
	}
	free(original_paths);
}
char *duplicate_cmd_and_free_paths(char *cmd, char **paths)
{
    char *result = ft_strdup(cmd);
    free_paths(paths, paths);
    return result;
}

t_env *find_envir_variable(t_ast *data, char *var_name, int len)
{
    t_env *current;

    current = data->env_list;
    while (current)
    {
        if (current->key && ft_strncmp(current->key, var_name, len) == 0)
            return (current);
        current = current->next;
    }
    return (NULL);
}

char *find_executable_path(t_ast *data, char *cmd)
{
    if (ft_strchr(cmd, '/')) {
        if (access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
        return NULL;
    }

    t_env *path_env = find_envir_variable(data, "PATH", 4);
    if (!path_env || !path_env->value)
        return NULL;

    char **paths = ft_split(path_env->value, ':');
    char **original_paths = paths;

    while (*paths)
    {
        char *full_path = ft_strjoin3(*paths, "/", cmd);
        if (access(full_path, X_OK) == 0)
        {
            char **temp = original_paths;
            while (*temp)
            {
                if (temp != paths)
                    free(*temp);
                temp++;
            }
            free(original_paths);
            return full_path;
        }
        free(full_path);
        paths++;
    }

    free_paths(original_paths, original_paths);
    return NULL;
}