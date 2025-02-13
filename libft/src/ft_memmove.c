/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 22:04:08 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 17:15:17 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_memmove - Copy memory area with overlapping regions.
 * @dst: The destination buffer where the content is to be copied.
 * @src: The source buffer from where the content is to be copied.
 * @len: The number of bytes to be copied.
 *
 * Description: This function copies 'len' bytes from memory area 'src' to
 * memory area 'dst'. The memory areas may overlap, and the copy is done
 * in a safe way.
 *
 * Return: A pointer to the destination buffer 'dst'.
 */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*dest_ptr;
	unsigned char		*src_ptr;

	i = 0;
	dest_ptr = (unsigned char *)dest;
	src_ptr = (unsigned char *)src;
	if (!dest && !src)
		return (NULL);
	if (dest == src)
		return (dest);
	if (dest > src)
	{
		while (n--)
			dest_ptr[n] = src_ptr[n];
	}
	else
	{
		while (i < n)
		{
			dest_ptr[i] = src_ptr[i];
			i++;
		}
	}
	return (dest);
}
