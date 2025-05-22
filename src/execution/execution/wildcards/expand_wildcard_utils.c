/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:36:52 by vszpiech          #+#    #+#             */
/*   Updated: 2025/04/12 15:11:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"
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

void	split_pattern(const char *pattern, char **dir_part,
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
