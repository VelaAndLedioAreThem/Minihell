/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:39:42 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 20:37:55 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_putstr_fd - Output a string to a file descriptor.
 * @s: The string to be output.
 * @fd: The file descriptor on which to write.
 *
 * Description: This function writes the string 's' to the given file
 * descriptor 'fd'. It does not add a newline character.
 *
 * Return: None.
 */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (!s || !fd)
		return ;
	write(fd, s, ft_strlen(s));
}

/**int main()
{
	ft_putstr_fd("Hello World", 1);
}*/