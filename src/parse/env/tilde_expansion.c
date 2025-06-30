/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 16:27:12 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/30 16:27:38 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include <pwd.h>

static char	*tilde_join_path(const char *prefix, const char *rest)
{
	char	*tmp;

	if (!prefix)
		return (NULL);
	tmp = ft_strjoin(prefix, rest);
	return (tmp);
}

char	*expand_tilde(const char *path, t_env *env)
{
    const char	*rest;
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

t_env	*create_copy_env_node(t_env *original)
{
	t_env	*env_node;

	env_node = malloc(sizeof(t_env));
	if (!env_node)
		return (NULL);
	env_node->key = NULL;
	env_node->next = NULL;
	env_node->value = NULL;
	if (original->key)
	{
		env_node->key = ft_strdup(original->key);
		if (!env_node->key)
		{
			free(env_node);
			return (NULL);
		}
	}
	if (original->value)
	{
		env_node->value = ft_strdup(original->value);
		if (!env_node->value)
			return (free(env_node->key), free(env_node), NULL);
	}
	return (env_node);
}

t_env	*deep_copy_env_list(t_env *env_list)
{
	t_env	*new_head;
	t_env	*new_curr;
	t_env	*new_node;
	t_env	*original;

	new_head = NULL;
	new_curr = NULL;
	original = env_list;
	while (original)
	{
		new_node = create_copy_env_node(original);
		if (!new_node)
		{
			free_env_list(new_head);
			return (NULL);
		}
		if (!new_head)
			new_head = new_node;
		else
			new_curr->next = new_node;
		new_curr = new_node;
		original = original->next;
	}
	return (new_head);
}
