/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:36:52 by vszpiech          #+#    #+#             */
/*   Updated: 2025/04/10 21:30:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include <dirent.h>

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

	new_args = NULL;
	new_count = 0;
	process_all_arguments(args, &new_args, &new_count);
	return (finalize_args_array(new_args, new_count));
}

static int	is_hidden_file(char *pattern, const char *filename)
{
	return (filename[0] == '.' && (pattern[0] != '.' || ft_strcmp(pattern,
				".") == 0));
}
