/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vela <vela@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:36:52 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/06 18:41:18 by vela             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>

char	**handle_no_wildcard(char *pattern)
{
	char	**result;

	result = malloc(2 * sizeof(char *));
	if (!result)
		return (NULL);
	result[0] = ft_strdup(pattern);
	if (!result[0])
	{
		free(result);
		return (NULL);
	}
	result[1] = NULL;
	return (result);
}

static int	match_after_star(const char *pat, const char *str)
{
	while (*str)
	{
		if (match_pattern(pat, str) == 1)
			return (1);
		str++;
	}
	return (0);
}

int	match_pattern(const char *pat, const char *str)
{
	if (*pat == '\0')
		return (*str == '\0');
	if (*pat == '*')
	{
		while (*pat == '*')
			pat++;
		if (*pat == '\0')
			return (1);
		return (match_after_star(pat, str));
	}
	if (*pat == '?' || *pat == *str)
	{
		if (*str != '\0')
			return (match_pattern(pat + 1, str + 1));
	}
	return (0);
}

char	**process_dir_entries(DIR *dir, char *file_part, char *dir_part,
		int *count)
{
	struct dirent	*entry;
	char			**matches;
	char			*path;

	matches = malloc(sizeof(char *));
	if (matches == NULL)
		return (NULL);
	matches[0] = NULL;
	*count = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (!is_hidden_file(file_part, entry->d_name)
			&& match_pattern(file_part, entry->d_name) == 1)
		{
			path = join_path(dir_part, entry->d_name);
			if (path == NULL)
				return (matches);
			matches = add_match(matches, count, path);
			if (matches == NULL)
				return (NULL);
		}
		entry = readdir(dir);
	}
	return (matches);
}

char	**finalize_matches(char **matches, int count)
{
	int		i;
	int		j;
	char	*tmp;

	if (count == 0)
		return (free(matches), NULL);
	i = -1;
	while (++i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(matches[i], matches[j]) > 0)
			{
				tmp = matches[i];
				matches[i] = matches[j];
				matches[j] = tmp;
			}
			j++;
		}
	}
	return (matches);
}
