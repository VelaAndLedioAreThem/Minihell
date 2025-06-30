/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:36:52 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/25 13:08:18 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>

int	match_pattern1(const char *pattern, const char *text)
{
	if (!pattern || !text)
		return (0);
	while (*pattern)
	{
		if (*pattern == '*')
		{
			pattern++;
			while (*text && !match_pattern1(pattern, text))
				text++;
		}
		else if (*pattern == '?' || *pattern == *text)
		{
			pattern++;
			text++;
		}
		else
		{
			return (0);
		}
	}
	return (*text == '\0');
}

int	has_wildcard(const char *str)
{
	return (ft_strchr(str, '*') != NULL);
}

void	split_pattern(const char *pattern, char **dir_part, char **file_part)
{
	char	*last_slash;

	last_slash = ft_strrchr(pattern, '/');
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

char	*join_path(char *dir_part, char *name)
{
	size_t	len_dir;
	size_t	len_name;
	char	*path;

	if (ft_strcmp(dir_part, "") == 0 || ft_strcmp(dir_part, "./") == 0
		|| ft_strcmp(dir_part, ".") == 0)
		return (ft_strdup(name));
	len_dir = ft_strlen(dir_part);
	len_name = ft_strlen(name);
	path = malloc(len_dir + len_name + 2);
	if (path == NULL)
		return (NULL);
	ft_strcpy(path, dir_part);
	if (dir_part[len_dir - 1] != '/')
		ft_strcat(path, "/");
	ft_strcat(path, name);
	return (path);
}

char	**add_match(char **matches, int *count, char *path)
{
	char	**new_tab;

	new_tab = ft_realloc(matches, (*count + 1) * sizeof(char *), (*count + 2)
			* sizeof(char *));
	if (new_tab == NULL)
	{
		free(path);
		return (NULL);
	}
	matches = new_tab;
	matches[*count] = path;
	(*count)++;
	matches[*count] = NULL;
	return (matches);
}
