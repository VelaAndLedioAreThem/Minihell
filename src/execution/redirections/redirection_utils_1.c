/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 12:42:00 by user              #+#    #+#             */
/*   Updated: 2025/06/30 16:18:25 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_line_fd(int fd)
{
	char	*line;
	char	buf[1];
	size_t	len;
	ssize_t	rd;

	line = NULL;
	len = 0;
	while ((rd = read(fd, buf, 1)) > 0)
	{
		line = ft_realloc(line, len + 1, len + 2);
		if (!line)
			return (NULL);
		line[len++] = buf[0];
		if (buf[0] == '\n')
			break ;
	}
	if (rd <= 0 && len == 0)
		return (NULL);
	line[len] = '\0';
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

