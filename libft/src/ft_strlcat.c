/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 20:49:42 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 20:38:22 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_strlcat - Concatenate strings with size limit.
 * @dst: The destination buffer where the string is to be concatenated.
 * @src: The source string to be appended.
 * @dstsize: The size of the destination buffer.
 *
 * Description: This function appends the null-terminated string 'src' to
 * the end of 'dst'. It will append at most 'dstsize - strlen(dst) - 1' bytes,
 * null-terminating the result. If 'dstsize' is 0 or less than the length of
 * 'dst', it only returns the length of 'src' plus 'dstsize'.
 *
 * Return: The total length of the string it tried to create, which is the
 * initial length of 'dst' plus the length of 'src'.
 */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t		i;
	size_t		len_dest;
	size_t		len_src;

	i = 0;
	len_dest = ft_strlen(dest);
	len_src = ft_strlen(src);
	if (size == 0 || len_dest >= size)
		return (size + len_src);
	while (i < (size - len_dest - 1) && src[i] != '\0')
	{
		dest[len_dest + i] = src[i];
		i++;
	}
	dest[len_dest + i] = '\0';
	return (len_dest + len_src);
}

// int main()
// {
// 	const char src[] = "Workbitch, asda";
// 	char dest[20] = "U better";
// 	size_t result = ft_strlcat(dest, src, 14);
// 	printf("The source is %s\n", src);
// 	printf("The dest is %s\n", dest);
// 	printf("The length is %zu\n", result);
// }