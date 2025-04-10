/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:01:21 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 13:12:08 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Check if a character is alphanumeric (a-z, A-Z, 0-9).
 *
 * @param c The character to check.
 * @return Non-zero if 'c' is alphanumeric, 0 otherwise.
 *
 * @details Determines if the character 'c' is alphanumeric
 * (a-z, A-Z, 0-9) according to the ASCII table.
 */

#include "libft.h"

int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
		return (1);
	return (0);
}
