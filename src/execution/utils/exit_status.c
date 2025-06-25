/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:29:34 by vela              #+#    #+#             */
/*   Updated: 2025/06/25 12:35:58 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static 	int	g_last_exit_status;

void	update_last_exit_status(int status)
{
	g_last_exit_status = status;
}

int	get_last_exit_status(void)
{
	return (g_last_exit_status);
}
