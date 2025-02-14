/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:04:53 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 12:17:00 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Check if a character is a valid ASCII character (0-127).
 *
 * @param c The character to check.
 * @return Non-zero if 'c' is an ASCII character, 0 otherwise.
 *
 * @details Determines if the character 'c' is within the ASCII range (0-127).
 */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}
