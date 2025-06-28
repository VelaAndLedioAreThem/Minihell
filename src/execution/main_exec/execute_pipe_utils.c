/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:34:24 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/28 17:34:26 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipe(int fd[2])
{
	if (pipe(fd) == -1)
	{
		perror("bash: pipe");
		return (1);
	}
	return (0);
}

void	close_pipe(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}
