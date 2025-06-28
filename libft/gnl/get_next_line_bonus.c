/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:45:58 by ldurmish          #+#    #+#             */
/*   Updated: 2024/08/12 13:20:22 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	ft_read_list(int fd, t_list **head)
{
	char	*buffer;
	int		bytes_read;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
	{
		ft_clear_list(head, NULL, NULL, 0);
		return (-1);
	}
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		ft_add_node(head, buffer);
		if (ft_strchr(buffer, '\n'))
			break ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	if (bytes_read <= 0)
		return (bytes_read);
	return (1);
}

char	*ft_split_node(t_list *list)
{
	char	*line_part;
	char	*rest;
	int		len;
	int		i;

	len = 0;
	while (list->data[len] && list->data[len] != '\n')
		len++;
	i = 0;
	line_part = malloc(len + (list->data[len] == '\n') + 1 * sizeof(char));
	if (!line_part)
		return (NULL);
	while (i < len)
	{
		line_part[i] = list->data[i];
		i++;
	}
	if (list->data[len] == '\n')
		++len;
	line_part[i] = '\0';
	rest = ft_strdup(list->data + len);
	free(list->data);
	list->data = rest;
	list->found_newline = (ft_strchr(list->data, '\n') != NULL);
	return (line_part);
}

char	*process_newline(t_list *current, t_glfl *glfl)
{
	glfl->part = ft_split_node(current);
	if (glfl->line != NULL)
	{
		glfl->new_line = ft_strjoin(glfl->line, glfl->part);
		free(glfl->line);
		glfl->line = ft_strdup(glfl->new_line);
		free(glfl->new_line);
	}
	else
		glfl->line = ft_strdup(glfl->part);
	if (glfl->line != NULL)
	{
		glfl->tmp = ft_strdup(glfl->line);
		free(glfl->line);
		glfl->line = ft_strjoin(glfl->tmp, "\n");
		free(glfl->tmp);
	}
	free(glfl->part);
	return (glfl->line);
}

char	*ft_get_line_from_list(t_list **head, t_list *current)
{
	t_glfl	glfl;

	glfl.line = NULL;
	current = *head;
	while (current)
	{
		if (current->found_newline)
			return (process_newline(current, &glfl));
		glfl.tmp = ft_strdup(current->data);
		if (glfl.line == NULL)
			glfl.line = glfl.tmp;
		else
		{
			glfl.new_line = ft_strjoin(glfl.line, glfl.tmp);
			free(glfl.line);
			free(glfl.tmp);
			glfl.line = glfl.new_line;
		}
		if (!current->found_newline)
			ft_clear_list(head, current, NULL, 1);
		current = *head;
	}
	return (glfl.line);
}

char	*get_next_line(int fd)
{
	static t_list	*head[4096];
	char			*line;
	int				read_status;

	if (fd < 0 || fd > 4095 || BUFFER_SIZE <= 0 || read(fd, &line, 0) < 0)
	{
		ft_clear_list(&head[fd], NULL, NULL, 0);
		return (NULL);
	}
	read_status = ft_read_list(fd, &head[fd]);
	if (read_status < 0 || (head[fd] == NULL && read_status == 0))
		return (ft_clear_list(&head[fd], NULL, NULL, 0), NULL);
	line = ft_get_line_from_list(&head[fd], head[fd]);
	if (line != NULL && line[0] == '\0' && read_status == 0)
		return (free(line), ft_clear_list(&head[fd], NULL, NULL, 0), NULL);
	return (line);
}
