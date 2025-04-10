/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 15:27:44 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 17:15:28 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_putchar_fd - Outputs the character 'c' to the given file descriptor.
 * @c: The character to output.
 * @fd: The file descriptor on which to write.
 *
 * Description: This function writes the character 'c' to the file descriptor
 * specified by 'fd'. If writing fails, the function does not perform any
 * error handling; it is the caller's responsibility to handle errors
 * appropriately.
 *
 * Return: This function does not return a value.
 */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

/**int main()
{
	ft_putchar_fd('H', 1);
}**/