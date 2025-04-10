/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 23:04:53 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 13:20:20 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Trim leading and trailing whitespaces from a string.
 *
 * @param s The string to be trimmed.
 * @param set The set of characters to trim.
 * @return The trimmed string, or NULL if memory allocation fails.
 *
 * @details Allocates and returns a copy of the string 's' with the leading and
 * trailing characters from the 'set' removed.
 * The 'set' characters are considered to be whitespaces.
 * Returns NULL if memory allocation fails.
 */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t			start;
	size_t			end;
	size_t			length;
	char			*trim;

	start = 0;
	end = ft_strlen(s1);
	if (!s1 || !set)
		return (NULL);
	while (s1[start] != '\0' && ft_strchr(set, s1[start]))
		start++;
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	length = end - start;
	trim = malloc(sizeof(char) * length + 1);
	if (!trim)
		return (NULL);
	ft_strlcpy(trim, &s1[start], length + 1);
	return (trim);
}
