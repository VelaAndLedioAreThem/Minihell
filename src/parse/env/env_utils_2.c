/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 20:03:46 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/21 20:03:56 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*process_env_value(char *value, t_args *arg)
{
	char	*processed_value;

	if (arg->double_quotes)
	{
		processed_value = remove_quotes_and_paren(value);
		if (!processed_value)
			processed_value = ft_strdup(value);
	}
	else
		processed_value = safe_and_expand_var(value);
	return (processed_value);
}
