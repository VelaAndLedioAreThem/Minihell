/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:21:58 by ldurmish          #+#    #+#             */
/*   Updated: 2025/02/17 00:15:30 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	ft_isspace(int num)
{
	if (num == ' ' || num == '\n' || num == '\t'
		|| num == '\v' || num == '\f' || num == '\r')
		return (1);
	return (0);
}

int	is_operator(char c)
{
	if (c == '&' || c == '|' || c == '(' || c == ')'
		|| c == '>' || c == '<' || c == '*')
		return (1);
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	int			i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
