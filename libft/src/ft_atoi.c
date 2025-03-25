/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 23:49:38 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 17:39:05 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_atoi - Convert a string to an integer.
 * @str: The string to be converted.
 *
 * Description: This function takes a string 'str' and converts it to an
 * integer. The function discards any whitespace characters until the first
 * non-whitespace character is found. Then, it takes an optional plus or minus
 * sign followed by as many numerical digits as possible, and interprets them
 * as an integer. If the first non-whitespace character is not a valid integer,
 * the function returns 0.
 *
 * Return: The converted integer, or 0 if no valid conversion could be performed.
 */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int			res;
	int			sign;

	res = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	return (res * sign);
}

/**int main()
{
    printf("ft_atoi(\"42\"): %d\n", ft_atoi("42"));           
    printf("ft_atoi(\"   -42\"): %d\n", ft_atoi("   -42"));  
    printf("ft_atoi(\"4193 with words\"): %d\n", ft_atoi("4193 with words"));
    printf("ft_atoi(\"words and 987\"): %d\n", ft_atoi("words and 987"));     
    printf("ft_atoi(\"-91283472332\"): %d\n", ft_atoi("-91283472332")); 
    printf("ft_atoi(\"+\"): %d\n", ft_atoi("+")); 
    printf("ft_atoi(\"\"): %d\n", ft_atoi("")); 

    return 0;
}*/
