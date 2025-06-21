/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 22:25:21 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/21 22:25:39 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parse_setup_tokens(t_token **tokens)
{
	t_token	*curr;

	if (!tokens || !*tokens)
		return (0);
	curr = *tokens;
	skip_tree_whitespaces(&curr);
	*tokens = curr;
	if (!curr)
		return (0);
	return (1);
}
