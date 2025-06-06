/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils3.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vela <vela@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:36:52 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/06 18:48:32 by vela             ###   ########.fr       */
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
