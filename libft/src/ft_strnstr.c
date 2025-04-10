/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 22:27:39 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 20:38:44 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_strnstr - Locate a substring in a string with length limit.
 * @haystack: The string to be searched.
 * @needle: The substring to be located.
 * @len: The maximum number of characters to search.
 *
 * Description: This function locates the first occurrence of the null-terminated
 * string 'needle' in the string 'haystack', where not more than 'len' characters
 * are searched. Characters after a '\0' are not searched.
 *
 * Return: A pointer to the first character of the first occurrence of 'needle',
 * or NULL if 'needle' is not found.
 */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *subs, size_t len)
{
	size_t				i;
	size_t				j;

	i = 0;
	if (*subs == '\0')
		return ((char *)str);
	while (i < len && str[i] != '\0')
	{
		j = 0;
		while (str[i + j] == subs[j] && i + j < len)
		{
			if (subs[j + 1] == '\0')
				return ((char *)&str[i]);
			j++;
		}
		i++;
	}
	return (NULL);
}
