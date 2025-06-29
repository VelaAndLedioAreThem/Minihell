/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe copy.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:17:20 by vszpiech          #+#    #+#             */
/*   Updated: 2025/04/10 21:44:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     setup_pipe(int fd[2])
{
        if (g_ctx && g_ctx->pipe_count >= MAX_OPEN_PIPES)
        {
                ft_putendl_fd("bash: pipe: too many open pipes", STDERR_FILENO);
                return (1);
        }
        if (pipe(fd) == -1)
        {
                perror("bash: pipe");
                return (1);
        }
        if (g_ctx)
                g_ctx->pipe_count++;
        return (0);
}

void    close_pipe(int fd[2])
{
        if (fd[0] != -1)
                close(fd[0]);
        if (fd[1] != -1)
                close(fd[1]);
        if (g_ctx && g_ctx->pipe_count > 0)
                g_ctx->pipe_count--;
}
