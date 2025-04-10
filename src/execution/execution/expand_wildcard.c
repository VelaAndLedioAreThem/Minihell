/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:36:52 by vszpiech          #+#    #+#             */
/*   Updated: 2025/04/10 20:56:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include <dirent.h>

int	match_pattern(const char *pattern, const char *text)
{
	if (!pattern || !text)
		return (0);
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
			return (0);
		}
	}
	return (*text == '\0');
}

static int	has_wildcard(const char *str)
{
	return (ft_strchr(str, '*') != NULL);
}

static void	split_pattern(const char *pattern, char **dir_part,
		char **file_part)
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

static char	*build_full_path(char *dir, const char *file)
{
	if (ft_strcmp(dir, ".") == 0)
		return (ft_strdup(file));
	if (dir[ft_strlen(dir) - 1] == '/')
		return (ft_strjoin(dir, file));
	return (ft_strjoin3(dir, "/", file));
}

static void	sort_matches(char **matches, int count)
{
	char	*temp;
	int		i;
	int		j;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(matches[i], matches[j]) > 0)
			{
				temp = matches[i];
				matches[i] = matches[j];
				matches[j] = temp;
			}
			j++;
		}
		i++;
	}
}

static int	is_hidden_file(char *pattern, const char *filename)
{
	return (filename[0] == '.' && (pattern[0] != '.' || ft_strcmp(pattern,
				".") == 0));
}

char	**expand_wildcard(char *pattern)
{
	char	*dir_part;
	char	*file_part;
	DIR		*dir;
	char	**matches;
	int		count;

	split_pattern(pattern, &dir_part, &file_part);
	if (!has_wildcard(file_part) && !has_wildcard(dir_part))
	{
		free(dir_part);
		free(file_part);
		return (handle_no_wildcard(pattern));
	}
	dir = opendir(dir_part);
	if (!dir)
	{
		free(dir_part);
		free(file_part);
		return (NULL);
	}
	matches = process_dir_entries(dir, file_part, dir_part, &count);
	closedir(dir);
	free(dir_part);
	free(file_part);
	return (finalize_matches(matches, count));
}

static void	add_arg_to_args(char ***new_args, int *new_count, char *arg)
{
	*new_args = realloc(*new_args, (*new_count + 1) * sizeof(char *));
	(*new_args)[*new_count] = ft_strdup(arg);
	(*new_count)++;
}

static void	free_matches_array(char **matches)
{
	int	j;

	j = 0;
	while (matches[j])
	{
		free(matches[j]);
		j++;
	}
	free(matches);
}

char	**expand_wildcards_in_args(char **args)
{
	char	**new_args;
	int		new_count;
	int		i;
	int		j;
	char	**matches;

	new_args = NULL;
	new_count = 0;
	i = 0;
	while (args[i])
	{
		matches = expand_wildcard(args[i]);
		if (!matches)
			add_arg_to_args(&new_args, &new_count, args[i]);
		else
		{
			j = 0;
			while (matches[j])
			{
				add_arg_to_args(&new_args, &new_count, matches[j]);
				j++;
			}
			free_matches_array(matches);
		}
		i++;
	}
	new_args = realloc(new_args, (new_count + 1) * sizeof(char *));
	new_args[new_count] = NULL;
	return (new_args);
}