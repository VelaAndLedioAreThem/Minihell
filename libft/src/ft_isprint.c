/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 20:43:15 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 12:17:14 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Check if a character is a printable ASCII character (32-126).
 *
 * @param c The character to check.
 * @return Non-zero if 'c' is a printable ASCII character, 0 otherwise.
 *
 * @details Determines if the character 'c' is a printable ASCII character,
 * which includes characters from space (32) to tilde (126) inclusive.
 */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c >= ' ' && c <= '~')
		return (1);
	return (0);
}
