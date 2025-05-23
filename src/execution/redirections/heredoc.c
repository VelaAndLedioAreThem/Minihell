/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/03/25 13:17:20 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>

/* push `path` into data->heredoc_files (O(1) amortised) */
int	add_heredoc(t_ast *data, char *path)
{
	int		i;
	char	**new_files;

	if (data->heredoc_files == NULL)
	{
		data->heredoc_files = malloc(sizeof(char *) * 2);
		if (!data->heredoc_files)
			return (1);
		data->heredoc_files[0] = path;
		data->heredoc_files[1] = NULL;
	}
	else
	{
		i = 0;
		while (data->heredoc_files[i])
			i++;
		new_files = realloc(data->heredoc_files, sizeof(char *) * (i + 2));
		if (!new_files)
			return (1);
		new_files[i] = path;
		new_files[i + 1] = NULL;
		data->heredoc_files = new_files;
	}
	data->heredoc_count++;
	return (0);
}

void	free_heredoc_list(t_ast *data)
{
	int	i;

	i = -1;
	data->heredoc_files = NULL;
	data->heredoc_count = 0;
	while (++i < data->heredoc_count)
	{
		free(data->heredoc_files[i]);
		data->heredoc_files[i] = NULL;
	}
	free(data->heredoc_files);
}
