/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 16:57:25 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/30 17:13:57 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	is_arithmetic_expr_valid(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]) && str[i] != ' ' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '%'
			&& str[i] != '(' && str[i] != ')')
			return (0);
		i++;
	}
	return (1);
}

int	validate_assignment_value(char *assignment)
{
	const char	*eq;
	const char	*value;
	const char	*end;
	char		*inner;
	size_t		len;

	eq = ft_strchr(assignment, '=');
	if (!eq || !*(eq + 1))
		return (1);
	value = eq + 1;
	if (ft_strncmp(value, "((", 2) != 0)
		return (1);
	len = ft_strlen(value);
	if (len < 4)
		return (0);
	end = value + len - 1;
	if (*(end) != ')' || *(end - 1) != ')')
		return (0);
	inner = ft_substr(value + 2, 0, len - 4);
	if (!inner)
		return (0);
	if (!is_arithmetic_expr_valid(inner))
		return (free(inner), 0);
	free(inner);
	return (1);
}
