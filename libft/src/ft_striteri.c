/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 13:08:52 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 20:38:13 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief This function applies function "f" on each character of
 * the string passed as argument, passing its index as first argument.
 * Each character is passed by address to "f" to be modified if necessary.
 * 
 * @param s The string on which to iterate.
 * @param f The function to apply to each character.
 */

#include "libft.h"

/**void	to_upper(unsigned int i, char *str)
{
	if (*str >= 'a' && *str <= 'z')
	{
		*str -= 32;
	}
	printf("%d : %c\n", i, *str);
}**/

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int		i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}

/**int main()
{
	char s[] = "hello, world";
	printf("The first string is %s\n", s);
	ft_striteri(s, to_upper);
	return (0);	
}**/