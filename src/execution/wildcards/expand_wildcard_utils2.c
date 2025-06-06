/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vela <vela@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:36:52 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/06 15:23:40 by vela             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>

void	process_argument(char *arg, char ***new_args, int *new_count)
{
	char	**matches;
	int		j;

	matches = expand_wildcard(arg);
	if (!matches)
	{
		add_arg_to_args(new_args, new_count, arg);
	}
	else
	{
		j = 0;
		while (matches[j])
		{
			add_arg_to_args(new_args, new_count, matches[j]);
			j++;
		}
		free_matches_array(matches);
	}
}

void	process_all_arguments(char **args, char ***new_args, int *new_count)
{
	int	i;

	i = 0;
	while (args[i])
	{
		process_argument(args[i], new_args, new_count);
		i++;
	}
}

char	**finalize_args_array(char **args, int count)
{
	args = realloc(args, (count + 1) * sizeof(char *));
	args[count] = NULL;
	return (args);
}

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

int	match_pattern(const char *pat, const char *str)
{
	/* pattern exhausted → match only if str is also exhausted */
	if (*pat == '\0')
		return (*str == '\0');

	/* collapse consecutive '*' to a single '*' */
	if (*pat == '*')
	{
		while (*pat == '*')
			++pat;
		/* a terminal '*' always matches */
		if (*pat == '\0')
			return (1);
		/* try every suffix of str until we find one that the rest of pat matches */
		for (; *str; ++str)
			if (match_pattern(pat, str))
				return (1);
		return (0);
	}
	/* single-character wildcard or literal match */
	if (*pat == '?' || *pat == *str)
		return (*str && match_pattern(pat + 1, str + 1));

	/* mismatch */
	return (0);
}

char	**process_dir_entries(DIR *dir, char *file_part, char *dir_part,
		int *count)
{
	struct dirent	*entry;
	char			**matches;
	char			*full_path;

	matches = malloc(sizeof(char *));
	if (!matches)
		return (NULL);
	matches[0] = NULL;
	*count = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		if (is_hidden_file(file_part, entry->d_name))
			continue ;
		if (match_pattern(file_part, entry->d_name))
		{
			if (ft_strcmp(dir_part, "") == 0 || ft_strcmp(dir_part, "./") == 0
				|| ft_strcmp(dir_part, ".") == 0)
				full_path = ft_strdup(entry->d_name);
			else
			{
				full_path = malloc(ft_strlen(dir_part)
						+ ft_strlen(entry->d_name) + 2);
				if (!full_path)
					continue ;
				ft_strcpy(full_path, dir_part);
				if (dir_part[ft_strlen(dir_part) - 1] != '/')
					ft_strcat(full_path, "/");
				ft_strcat(full_path, entry->d_name);
			}
			matches = realloc(matches, (*count + 2) * sizeof(char *));
			if (!matches)
			{
				free(full_path);
				return (NULL);
			}
			matches[*count] = full_path;
			(*count)++;
			matches[*count] = NULL;
		}
	}
	return (matches);
}

char	**finalize_matches(char **matches, int count)
{
	int		i;
	int		j;
	char	*temp;

	if (count == 0)
	{
		free(matches);
		return (NULL);
	}
	for (i = 0; i < count - 1; i++)
	{
		for (j = i + 1; j < count; j++)
		{
			if (ft_strcmp(matches[i], matches[j]) > 0)
			{
				temp = matches[i];
				matches[i] = matches[j];
				matches[j] = temp;
			}
		}
	}
	return (matches);
}
