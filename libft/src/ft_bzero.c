/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 22:13:36 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 13:34:40 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_bzero - Zero a block of memory.
 * @s: The memory area to be zeroed.
 * @n: The number of bytes to be zeroed.
 *
 * Description: This function sets the first 'n' bytes of the memory area
 * pointed to by 's' to zero (bytes containing '\0').
 *
 * Return: None.
 */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char		*ptr;
	size_t				i;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		ptr[i] = 0;
		i++;
	}
}

/**int main()
{
	char s[] = "Hello";
	size_t n = 2;
	ft_bzero(s, n);
	printf("The string is ");
	for (size_t i = 0; i < sizeof(s); i++) {
		printf("%c", s[i]);
	}
	printf("\n");
}*/