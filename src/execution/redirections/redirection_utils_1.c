/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 12:42:00 by user              #+#    #+#             */
/*   Updated: 2025/06/30 17:17:49 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_line_fd(int fd)
{
	char	*line;
	ssize_t	bytes_read;
	char	buffer[1];
	size_t	length;

	line = NULL;
	length = 0;
	bytes_read = read(fd, buffer, 1);
	while (bytes_read > 0)
	{
		line = ft_realloc(line, length + 1, length + 2);
		if (line == NULL)
			return (NULL);
		line[length] = buffer[0];
		length++;
		if (buffer[0] == '\n')
			break ;
		bytes_read = read(fd, buffer, 1);
	}
	if (bytes_read <= 0 && length == 0)
		return (NULL);
	line[length] = '\0';
	return (line);
}

int	handle_heredocs(t_ast *data, t_redir_ls *list)
{
	t_redir_ls	*curr;

	curr = list;
	while (curr)
	{
		if (curr->type == TOKEN_HEREDOC)
			if (!create_heredoc_file(data, curr))
				return (0);
		curr = curr->next;
	}
	return (1);
}
