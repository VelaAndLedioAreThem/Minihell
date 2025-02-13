/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:21:11 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 13:19:55 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Concatenate two strings into a new allocated string.
 *
 * @param s1 The first string.
 * @param s2 The second string.
 * @return The newly allocated string resulting from the
 * concatenation of 's1' and 's2',
 * or NULL if memory allocation fails.
 *
 * @details Allocates a new string that is the concatenation of
 * 's1' and 's2'. 
 * The memory for the new string is dynamically allocated.
 * Returns NULL if memory allocation fails.
 */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t			index;
	size_t			total_length;
	char			*result;

	if (!s1 || !s2)
		return (NULL);
	total_length = ft_strlen(s1) + ft_strlen(s2) + 1;
	result = (char *)malloc(sizeof(char) * total_length);
	if (!result)
		return (NULL);
	index = 0;
	while (*s1)
		result[index++] = *s1++;
	while (*s2)
		result[index++] = *s2++;
	result[index] = '\0';
	return (result);
}
