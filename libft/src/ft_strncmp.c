/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:45:01 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 21:09:32 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_strncmp - Compare two strings up to a specified number of characters.
 * @s1: The first string to be compared.
 * @s2: The second string to be compared.
 * @n: The maximum number of characters to compare.
 *
 * Description: This function compares up to 'n' characters of the strings
 * 's1' and 's2'. The comparison is done lexicographically. It stops comparing
 * either after 'n' characters have been compared, or a null character is
 * encountered.
 *
 * Return: An integer less than, equal to, or greater than zero if 's1' is
 * found, respectively, to be less than, to match, or be greater than 's2'.
 */

#include "libft.h"

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (str1[i] != '\0' && str2[i] != '\0'
		&& str1[i] == str2[i] && i < n - 1)
		i++;
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}
