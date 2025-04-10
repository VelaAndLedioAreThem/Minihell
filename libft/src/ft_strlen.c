/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 20:54:12 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 13:09:49 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_strlen - Calculate the length of a string.
 * @str: The string to calculate the length of.
 *
 * Description: This function takes a string 'str' and returns its
 * length, not including the terminating null byte ('\0'). The function
 * iterates through the string until it encounters the null byte.
 *
 * Return: The length of the string.
 */

#include "libft.h"

int	ft_strlen(const char *str)
{
	int		len;

	len = 0;
	while (str[len] != '\0')
		len++;
	return (len);
}

/**int main()
{
    char *str = "Hello, world!";
    size_t len = ft_strlen(str);
    printf("Length of '%s' is %zu\n", str, len);
    return 0;
}*/