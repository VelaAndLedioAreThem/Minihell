#include "../../../include/minishell.h"
#include <pwd.h>

static char *tilde_join_path(const char *prefix, const char *rest)
{
    char *tmp;
    if (!prefix)
        return NULL;
    tmp = ft_strjoin(prefix, rest);
    return tmp;
}

char *expand_tilde(const char *path, t_env *env)
{
    const char *rest;
    char *prefix;
    struct passwd *pw;
    char *username;

    if (!path || path[0] != '~')
        return path ? ft_strdup(path) : NULL;
    rest = path + 1;
    prefix = NULL;
    if (*rest == '\0' || *rest == '/')
        prefix = get_env_value(env, "HOME");
    else if (*rest == '+')
    {
        rest++;
        prefix = get_env_value(env, "PWD");
    }
    else if (*rest == '-')
    {
        rest++;
        prefix = get_env_value(env, "OLDPWD");
    }
    else
    {
        const char *slash = ft_strchr(rest, '/');
        size_t len;

        if (slash)
                len = (size_t)(slash - rest);
        else
                len = ft_strlen(rest);
        username = ft_substr(rest, 0, len);
        pw = getpwnam(username);
        free(username);
        if (!pw)
        {
            ft_putendl_fd("minishell: no such user", STDERR_FILENO);
            return NULL;
        }
        prefix = pw->pw_dir;
        rest = ft_strchr(rest, '/');
        if (!rest)
            rest = "";
    }
    if (!prefix)
        return NULL;
    return tilde_join_path(prefix, rest);
}

void expand_tilde_tokens(t_token *tokens, t_env *env)
{
    t_token *curr;
    char *expanded;

    curr = tokens;
    while (curr)
    {
        if (curr->type == TOKEN_WORD && !curr->quotes.in_single_quotes && curr->value && curr->value[0] == '~')
        {
            expanded = expand_tilde(curr->value, env);
            if (expanded)
            {
                free(curr->value);
                curr->value = expanded;
            }
        }
        curr = curr->next;
    }
}
