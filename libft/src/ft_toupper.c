/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 21:52:52 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 20:39:09 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_toupper - Convert a lowercase letter to an uppercase letter.
 * @c: The character to be converted.
 *
 * Description: This function converts a lowercase letter to the
 * corresponding uppercase letter. If the character is not a lowercase
 * letter, it is returned unchanged.
 *
 * Return: The uppercase equivalent of the character if it is a lowercase
 * letter, otherwise the character itself.
 */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return (c);
}
