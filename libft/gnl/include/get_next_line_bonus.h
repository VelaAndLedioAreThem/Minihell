/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_lines_bonus.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 19:39:52 by ldurmish          #+#    #+#             */
/*   Updated: 2024/08/11 18:01:10 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_list
{
	char			*data;
	int				len;
	struct s_list	*next;
	int				to_delete;
	int				found_newline;
}				t_list;

typedef struct s_glfl
{
	char		*tmp;
	char		*line;
	char		*new_line;
	char		*part;
	t_list		*temp;
}			t_glfl;

char			*get_next_line(int fd);
char			*ft_strdup(const char *str);
char			*ft_strjoin(char *s1, char *s2);
void			ft_add_node(t_list **head, char *str);
void			ft_remove_data(t_list **head);
void			*ft_clear_list(t_list **list, t_list *clean_node, char *buf,
					int del_one);
char			*ft_strchr(const char *s, int c);

#endif
