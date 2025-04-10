/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 20:09:16 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 21:35:22 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_strlcpy - Copy a string to a buffer with size limit.
 * @dst: The destination buffer where the string is to be copied.
 * @src: The source string to be copied.
 * @dstsize: The size of the destination buffer.
 *
 * Description: This function copies up to 'dstsize - 1' characters from
 * the null-terminated string 'src' to 'dst', null-terminating the result.
 * If 'dstsize' is 0, the function does nothing.
 *
 * Return: The total length of the string it tried to create, which is
 * the length of 'src'.
 */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	src_len;

	src_len = ft_strlen(src);
	i = 0;
	if (!dest || !src)
		return (0);
	if (size == 0)
		return (ft_strlen(src));
	while (src[i] != '\0' && i < size - 1)
	{
		dest[i] = src[i];
		i++;
	}
	if (size != 0)
		dest[i] = '\0';
	return (src_len);
}
