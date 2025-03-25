/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:53:09 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 13:14:47 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Check if a character is a digit (0-9).
 *
 * @param c The character to check.
 * @return Non-zero if 'c' is a digit, 0 otherwise.
 *
 * @details Determines if the character 'c'\
 * is a digit (0-9) according to the ASCII table.
 */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

/**int main()
{
    char ch = '7';
    if (ft_isdigit(ch))
        printf("Character '%c' is a digit: true\n", ch);
    else
        printf("Character '%c' is a digit: false\n", ch);
    
    return 0;
}*/