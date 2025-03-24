#include "../../include/minishell.h"
#include <stdlib.h>
#include <unistd.h>

// Add this to your utils if missing

char *ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
    char *tmp = ft_strjoin(s1, s2);
    char *res = ft_strjoin(tmp, s3);
    free(tmp);
    return res;
}


// Updated print_env_list
void print_env_list(t_env *env_list, int fd_out)
{
    while (env_list)
    {
        ft_putstr_fd(env_list->key, fd_out);       // Changed var_name->key
        ft_putstr_fd("=", fd_out);
        ft_putendl_fd(env_list->value ? env_list->value : "", fd_out); // Changed var_value->value
        env_list = env_list->next;
    }
}

// Fixed env function
char **env(t_env **lst)
{
    t_env *tmp = *lst;
    int count = 0;
    
    // Count all environment entries
    while (tmp) {
        count++;
        tmp = tmp->next;
    }

    char **envp = malloc((count + 1) * sizeof(char *));
    tmp = *lst;
    int i = 0;
    while (tmp) {
        envp[i++] = ft_strjoin3(tmp->key, "=", tmp->value); // Updated member names
        tmp = tmp->next;
    }
    envp[i] = NULL;
    return envp;
}

// Updated environment variable functions
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
        if (!ft_strcmp(tmp->key, "SHLVL"))  // Changed var_name->key
        {
            free(tmp->value);
            tmp->value = ft_itoa(shlvl);    // Changed var_value->value
            return;
        }
        tmp = tmp->next;
    }
    
    // Create new SHLVL entry if missing
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
        if (ft_strncmp(current->key, var_name, len) == 0)
            return (current);
        current = current->next;
    }
    return (NULL);
}

char *find_executable_path(t_ast *data, char *cmd)
{
    char    *tmp;
    char    *command;
    char    **paths;
    t_env   *path;
    char    **original_paths;

    path = find_envir_variable(data, "PATH", 4);
    if (!path || !path->value)
        return (NULL);
    paths = ft_split(path->value, ':');
    original_paths = paths;
    while (*paths)
    {
        if (access(cmd, X_OK) == 0)
            return (duplicate_cmd_and_free_paths(cmd, original_paths));
        tmp = ft_strjoin(*paths, "/");
        command = ft_strjoin(tmp, cmd);
        ft_strdel(&tmp);
        if (access(command, F_OK) == 0)
        {
            free_paths(paths, original_paths);
            return (command);
        }
        ft_strdel(&command);
        paths++;
    }
    free_paths(original_paths, original_paths);
    return (NULL);
}