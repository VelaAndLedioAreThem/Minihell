/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 19:04:01 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 17:15:34 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_putendl_fd - Output a string to a file descriptor, followed by a newline.
 * @s: The string to be output.
 * @fd: The file descriptor on which to write.
 *
 * Description: This function writes the string 's' to the given file
 * descriptor 'fd', followed by a newline character. It first checks if
 * the string is null or if the file descriptor is invalid. If either
 * condition is true, it returns without doing anything.
 *
 * Return: None.
 */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	if (!s || fd < 0)
		return ;
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}
