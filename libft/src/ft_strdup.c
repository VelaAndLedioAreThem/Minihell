/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 00:10:24 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 20:38:08 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_strdup - Duplicate a string.
 * @s1: The string to be duplicated.
 *
 * Description: This function takes a string 's1' and allocates memory
 * to create a copy of it. The copy is created and a pointer to it is
 * returned. The memory for the new string is dynamically allocated
 * and should be freed by the caller. If the allocation fails, the
 * function returns NULL.
 *
 * Return: A pointer to the duplicated string, or NULL if memory
 * allocation fails.
 */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int			len;
	char		*result;

	len = ft_strlen(s);
	result = ((char *)malloc(sizeof(char) * (len + 1)));
	if (result == NULL)
		return (NULL);
	len = 0;
	while (s[len] != '\0')
	{
		result[len] = s[len];
		len++;
	}
	result[len] = '\0';
	return (result);
}
