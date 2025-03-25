/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:21:34 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 20:38:49 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_strrchr - Locate the last occurrence of a character in a string.
 * @s: The string to be searched.
 * @c: The character to be located.
 *
 * Description: This function searches for the last occurrence of the
 * character 'c' in the string 's'. The terminating null byte is considered
 * part of the string, so if 'c' is '\0', the function locates the
 * terminating '\0'.
 *
 * Return: A pointer to the last occurrence of the character 'c' in the
 * string 's', or NULL if the character is not found.
 */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int			i;
	char		*ptr;

	i = ft_strlen(str);
	while (i >= 0)
	{
		if (str[i] == (char)c)
		{
			ptr = (char *)&str[i];
			return (ptr);
		}
		i--;
	}
	return (NULL);
}
