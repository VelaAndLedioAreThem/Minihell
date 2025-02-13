/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:28:49 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/21 14:09:53 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_strmapi - Apply a function to each character of a string.
 * @s: The input string.
 * @f: The function to apply to each character.
 *
 * Description: Applies the function 'f' to each character of the string 's',
 * passing the character's index as the first argument to 'f', and creates a new
 * string with the results. The new string is dynamically allocated and must be
 * freed by the caller.
 *
 * Return: A pointer to the new string, or NULL if memory allocation fails.
 */

#include "libft.h"

/**char	ft_transform(unsigned int i, char c)
{
	if (i % 2 == 0)
		return (ft_toupper(c));
	else
		return (ft_tolower(c));
	return (c);
}**/

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t				i;
	size_t				length;
	char				*str;

	if (!s || !f)
		return (NULL);
	length = ft_strlen(s);
	str = (char *)malloc(sizeof(char) * (length + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < length)
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}

/**int main()
{
	char original[] = "Hello, WORld";
	char *transformed = ft_strmapi(original, ft_transform);
	if (transformed)
	{
		printf("The original string is %s\n", original);
		printf("The transformed string is %s\n", transformed);
		free(transformed);
	}
}
**/
