/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:36:52 by vszpiech          #+#    #+#             */
/*   Updated: 2025/03/24 19:37:27 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

#include <dirent.h>


int match_pattern(const char *pattern, const char *text)
{
    if (!pattern || !text) return 0;
    
    while (*pattern)
    {
        if (*pattern == '*')
        {
            pattern++;
            while (*text && !match_pattern(pattern, text))
                text++;
        }
        else if (*pattern == '?' || *pattern == *text)
        {
            pattern++;
            text++;
        }
        else
        {
            return 0;
        }
    }
    return *text == '\0';
}

static int has_wildcard(const char *str) {
    return (ft_strchr(str, '*') != NULL);
}

static void split_pattern(const char *pattern, char **dir_part, char **file_part)
{
    char *last_slash = ft_strrchr(pattern, '/');
    
    if (last_slash)
    {
        *dir_part = ft_strndup(pattern, last_slash - pattern + 1);
        *file_part = ft_strdup(last_slash + 1);
    }
    else
    {
        *dir_part = ft_strdup(".");
        *file_part = ft_strdup(pattern);
    }
}

static char *build_full_path( char *dir, const char *file)
{
    if (ft_strcmp(dir, ".") == 0)
        return ft_strdup(file);
    if (dir[ft_strlen(dir)-1] == '/')
        return ft_strjoin(dir, file);
    return ft_strjoin3(dir, "/", file);
}

static void sort_matches(char **matches, int count) {
    int i, j;
    char *temp;

    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (ft_strcmp(matches[i], matches[j]) > 0) {
                temp = matches[i];
                matches[i] = matches[j];
                matches[j] = temp;
            }
        }
    }
}

static int is_hidden_file(char *pattern, const char *filename) {
    return (filename[0] == '.' && (pattern[0] != '.' || ft_strcmp(pattern, ".") == 0));
}

char **expand_wildcard(char *pattern) {
    char *dir_part;
    char *file_part;
    DIR *dir;
    struct dirent *entry;

    split_pattern(pattern, &dir_part, &file_part);
    
    if (!has_wildcard(file_part) && !has_wildcard(dir_part))
    {
        free(dir_part);
        free(file_part);
        char **result = ft_calloc(2, sizeof(char *));
        result[0] = ft_strdup(pattern);
        return result;
    }

    dir = opendir(dir_part);
    if (!dir)
    {
        free(dir_part);
        free(file_part);
        return NULL;
    }

    char **matches = NULL;
    int count = 0;
    
    while ((entry = readdir(dir)) != NULL)
    {
        if (is_hidden_file(file_part, entry->d_name))
            continue;
        if (match_pattern(file_part, entry->d_name))
        {
            char *full_path = build_full_path(dir_part, entry->d_name);
            matches = realloc(matches, (count + 1) * sizeof(char *));
            matches[count++] = full_path;
        }
    }
    closedir(dir);

    if (count == 0) {
        free(matches);
        matches = NULL;
    } else {
        sort_matches(matches, count);
        matches = realloc(matches, (count + 1) * sizeof(char *));
        matches[count] = NULL;
    }

    free(dir_part);
    free(file_part);
    return matches;
}

char **expand_wildcards_in_args(char **args) {
    char **new_args = NULL;
    int new_count = 0;

    for (int i = 0; args[i]; i++) {
        char **matches = expand_wildcard(args[i]);
        if (!matches) {
            new_args = realloc(new_args, (new_count + 1) * sizeof(char *));
            new_args[new_count++] = ft_strdup(args[i]);
        } else {
            for (int j = 0; matches[j]; j++) {
                new_args = realloc(new_args, (new_count + 1) * sizeof(char *));
                new_args[new_count++] = ft_strdup(matches[j]);
            }
            for (int j = 0; matches[j]; j++) free(matches[j]);
            free(matches);
        }
    }
    new_args = realloc(new_args, (new_count + 1) * sizeof(char *));
    new_args[new_count] = NULL;
    return new_args;
}