/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 20:48:01 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 21:45:16 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_itoa - Convert an integer to a null-terminated string.
 * @n: The integer to be converted.
 *
 * Description: This function takes an integer 'n' and converts it to a string
 * representation. Memory for the resulting string is dynamically allocated
 * and needs to be freed by the caller. If the allocation fails, the function
 * returns NULL. The function handles both positive and negative integers.
 *
 * Return: A pointer to the resulting null-terminated string, or NULL if memory
 * allocation fails.
 */

#include "libft.h"

static	int	ft_num_len(int num)
{
	int		len;

	len = 0;
	if (num <= 0)
		len = 1;
	while (num)
	{
		num /= 10;
		len++;
	}
	return (len);
}

static	char	*ft_allocate_length(int length)
{
	char	*str;

	str = (char *)malloc(sizeof(char) * (length + 1));
	if (str == NULL)
		return (NULL);
	return (str);
}

char	*ft_itoa(int n)
{
	int		length;
	char	*str;
	long	num;

	num = n;
	length = ft_num_len(n);
	str = ft_allocate_length(length);
	if (!str)
		return (NULL);
	str[length--] = '\0';
	if (num < 0)
	{
		str[0] = '-';
		num = -num;
	}
	if (num == 0)
		str[0] = '0';
	while (num)
	{
		str[length] = (num % 10) + '0';
		num /= 10;
		length--;
	}
	return (str);
}

/**int main()
{
	int	num = -2147483648;
	char *str = ft_itoa(num);
	printf("The number is %d\n", num);
	if (str)
	{
		printf("The number is %s\n", str);
		free(str);
	}
	return (0);
}
**/
