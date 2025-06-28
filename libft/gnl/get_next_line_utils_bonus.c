/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 23:15:41 by ldurmish          #+#    #+#             */
/*   Updated: 2024/08/13 20:55:22 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_strdup(const char *str)
{
	int			len;
	char		*new_str;

	if (!str)
		return (NULL);
	len = 0;
	new_str = NULL;
	while (str[len] != '\0')
		len++;
	new_str = (char *)malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return (NULL);
	len = 0;
	while (str[len] != '\0')
	{
		new_str[len] = str[len];
		len++;
	}
	new_str[len] = '\0';
	return (new_str);
}

void	ft_add_node(t_list **head, char *str)
{
	t_list	*new_node;
	t_list	**current;

	if (!str)
		return ;
	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return ;
	new_node->data = ft_strdup(str);
	if (!new_node->data)
	{
		free(new_node);
		return ;
	}
	new_node->found_newline = (ft_strchr(new_node->data, '\n') != NULL);
	new_node->next = NULL;
	current = head;
	while (*current)
		current = &((*current)->next);
	*current = new_node;
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t			index;
	size_t			total_length;
	char			*joined;
	size_t			length_str1;
	size_t			length_str2;

	if (!s1 || !s2)
		return (NULL);
	length_str1 = 0;
	while (s1[length_str1] != '\0')
		length_str1++;
	length_str2 = 0;
	while (s2[length_str2] != '\0')
		length_str2++;
	total_length = length_str1 + length_str2 + 1;
	joined = (char *)malloc(sizeof(char) * total_length);
	if (!joined)
		return (NULL);
	index = 0;
	while (*s1)
		joined[index++] = *s1++;
	while (*s2)
		joined[index++] = *s2++;
	joined[index] = '\0';
	return (joined);
}

char	*ft_strchr(const char *s, int c)
{
	int			i;

	i = 0;
	while (s[i] != '\0' && s[i] != (char)c)
		i++;
	if (s[i] == (char)c)
		return ((char *)&s[i]);
	return (0);
}

void	*ft_clear_list(t_list **list, t_list *clean_node, char *buf,
	int del_one)
{
	t_list	*tmp;

	if (del_one)
	{
		tmp = *list;
		*list = (*list)->next;
		return (free(tmp->data), free(tmp), NULL);
	}
	while (*list)
	{
		tmp = (*list)->next;
		free((*list)->data);
		free(*list);
		*list = tmp;
	}
	*list = NULL;
	if (clean_node != NULL && clean_node->data != NULL
		&& clean_node->data[0] != '\0')
		*list = clean_node;
	else
	{
		free(buf);
		free(clean_node);
	}
	return (NULL);
}
