/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:36:52 by vszpiech          #+#    #+#             */
/*   Updated: 2025/04/10 21:26:51 by marvin           ###   ########.fr       */
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
