/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 23:15:36 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 17:15:07 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_memcpy - Copy memory area.
 * @dst: The destination buffer where the content is to be copied.
 * @src: The source buffer from where the content is to be copied.
 * @n: The number of bytes to be copied.
 *
 * Description: This function copies 'n' bytes from memory area 'src' to
 * memory area 'dst'. The memory areas must not overlap. Use ft_memmove if
 * the memory areas do overlap.
 *
 * Return: A pointer to the destination buffer 'dst'.
 */

#include "libft.h"

void	*ft_memcpy(void	*dest, void const *src, size_t n)
{
	size_t					i;
	unsigned char			*dest_ptr;
	unsigned char			*src_ptr;

	i = 0;
	dest_ptr = (unsigned char *)dest;
	src_ptr = (unsigned char *)src;
	if (dest == src || n == 0)
		return (dest);
	if (!dest && !src)
		return (0);
	while (i < n)
	{
		dest_ptr[i] = src_ptr[i];
		i++;
	}
	return (dest);
}
