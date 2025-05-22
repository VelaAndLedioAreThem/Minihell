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
int	add_heredoc_file(t_ast *data, char *path)
{
	char	**new;
	int		i;

	if (data->heredoc_count + 1 > data->heredoc_cap)
	{
		data->heredoc_cap = (data->heredoc_cap == 0) ? 4 : data->heredoc_cap * 2;
		new = (char **)malloc(sizeof(char *) * data->heredoc_cap);
		if (!new)
			return (1);
		i = -1;
		while (++i < data->heredoc_count)
			new[i] = data->heredoc_files[i];
		free(data->heredoc_files);
		data->heredoc_files = new;
	}
	data->heredoc_files[data->heredoc_count++] = path;
	return (0);
}

void	free_heredoc_list(t_ast *data)
{
	int i;

	i = -1;
	while (++i < data->heredoc_count)
		free(data->heredoc_files[i]);
	free(data->heredoc_files);
}
