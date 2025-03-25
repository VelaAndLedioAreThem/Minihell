/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:28:46 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 13:23:23 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Extract a substring from a string.
 *
 * @param s The string from which to extract the substring.
 * @param start The starting index of the substring in 's'.
 * @param len The maximum length of the substring.
 * @return The substring extracted from 's', or NULL
 * If memory allocation fails or 'start' is out of range.
 *
 * @details Allocates and returns a substring from the string 's' 
 * beginning at index 'start' for up to 'len' characters.
 * The substring is terminated with '\0'. 
 * If 'start' is beyond the end of 's', the function returns NULL. 
 * Returns NULL if memory allocation fails.
 */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char			*substring;
	size_t			str_len;
	size_t			sub_length;

	if (!s)
		return (NULL);
	str_len = ft_strlen(s);
	if (start >= str_len)
		return (ft_strdup(""));
	if (len < str_len - start)
		sub_length = len;
	else
		sub_length = str_len - start;
	substring = (char *)malloc(sizeof(char) * (sub_length + 1));
	if (!substring)
		return (NULL);
	ft_strlcpy(substring, s + start, sub_length + 1);
	return (substring);
}
/**int main()
{
    char *s = "Hello, world!";
    char *substr;

    substr = ft_substr(s, 7, 5);
    printf("Substring 1: \"%s\"\n", substr); // Expected: "world"
    free(substr);

    substr = ft_substr(s, 0, 20);
    printf("Substring 2: \"%s\"\n", substr); // Expected: "Hello, world!"
    free(substr);

    substr = ft_substr(s, 15, 5);
    printf("Substring 3: \"%s\"\n", substr); // Expected: ""
    free(substr);

    substr = ft_substr(NULL, 0, 5);
    printf("Substring 4: \"%s\"\n", substr); // Expected: NULL
    free(substr);

    return 0;
}*/
