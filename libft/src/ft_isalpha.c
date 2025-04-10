/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:43:31 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/18 17:14:04 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_isalpha - Check if a character is alphabetic.
 * @c: The character to be checked.
 *
 * Description: This function checks if the character 'c' is an alphabetic
 * letter. An alphabetic letter is defined as either an uppercase letter
 * (A-Z) or a lowercase letter (a-z).
 *
 * Return: 1 if the character is alphabetic, 0 otherwise.
 */

#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}
