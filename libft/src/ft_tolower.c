/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 22:10:30 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 20:39:05 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_tolower - Convert an uppercase letter to a lowercase letter.
 * @c: The character to be converted.
 *
 * Description: This function converts an uppercase letter to the
 * corresponding lowercase letter. If the character is not an uppercase
 * letter, it is returned unchanged.
 *
 * Return: The lowercase equivalent of the character if it is an uppercase
 * letter, otherwise the character itself.
 */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}
