/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 12:55:49 by ipetruni          #+#    #+#             */
/*   Updated: 2025/03/07 19:22:06 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_countarr_args(char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && ((s[i + 1] == c && !in_quotes(s, i + 1))
				|| s[i + 1] == 0))
			count++;
		i++;
	}
	return (count);
}

int	ft_arrarr_args(char **arr, char *s, char c, int str)
{
	int	a;
	int	z;

	a = 0;
	z = 0;
	while (s[z])
	{
		if (s[z] == c && !in_quotes(s, z))
			a = z + 1;
		if (s[z] != c && ((s[z + 1] == c && !in_quotes(s, z + 1)) || !s[z + 1]))
		{
			arr[str] = ft_calloc((z - a + 2), sizeof(char));
			if (!arr[str])
			{
				while (str--)
					free(arr[str]);
				return (0);
			}
			ft_strlcpy(arr[str], s + a, z - a + 2);
			str++;
		}
		z++;
	}
	arr[str] = NULL;
	return (1);
}

char	**ft_split_args(char *s, char c)
{
	char	**arr;

	if (!s)
		return (NULL);
	arr = ft_calloc((ft_countarr_args(s, c) + 1), sizeof(char *));
	if (!arr)
		return (NULL);
	if (!ft_arrarr_args(arr, s, c, 0))
	{
		free(arr);
		return (NULL);
	}
	return (arr);
}

int	arraylen(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
char *ft_strjoin_free(char *s1, char *s2)
{
    char *result = ft_strjoin(s1, s2);
    free(s1);
    return result;
}