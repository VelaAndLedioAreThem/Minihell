/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:23:23 by vszpiech          #+#    #+#             */
/*   Updated: 2025/02/13 18:21:01 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

void	handle_redirections(char **args)
{
	int		fd;

	char	*delim;
	char    tmpname[] = "/tmp/heredocXXXXXX";
	int		tmpfd;
	char    *line;

	for (int i = 0; args[i]; i++)
	{
		if (!my_strcmp(args[i], ">"))
		{
			fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror("open");
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			args[i] = NULL;
			break ;
		}
		else if (!my_strcmp(args[i], ">>"))
		{
			fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror("open");
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			args[i] = NULL;
			break ;
		}
		else if (!my_strcmp(args[i], "<"))
		{
			fd = open(args[i + 1], O_RDONLY);
			if (fd == -1)
			{
				perror("open");
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
			args[i] = NULL;
			break ;
		}
		else if (!my_strcmp(args[i], "<<"))
		{
            g_shell.in_heredoc = 1; // Set heredoc flag
			delim = args[i + 1];
			tmpfd = mkstemp(tmpname);
			if (tmpfd == -1)
			{
				perror("mkstemp");
				exit(EXIT_FAILURE);
			}
			unlink(tmpname);
			while (!g_shell.signal_received)
			{
				line = readline("> ");
				if (!line || g_shell.signal_received) 
                {
                    break;
                }
				if (my_strcmp(line, delim) == 0)
				{
					free(line);
					break ;
				}
                
				write(tmpfd, line, strlen(line));
				write(tmpfd, "\n", 1);
				free(line);
			}
			lseek(tmpfd, 0, SEEK_SET);
			dup2(tmpfd, STDIN_FILENO);
            g_shell.in_heredoc = 0;
            g_shell.signal_received = 0;
			close(tmpfd);
			args[i] = NULL;
			break ;
		}
	}
}
