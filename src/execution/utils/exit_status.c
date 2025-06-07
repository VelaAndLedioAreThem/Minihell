/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vela <vela@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:29:34 by vela              #+#    #+#             */
/*   Updated: 2025/06/07 09:56:29 by vela             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	g_last_exit_status;

void	update_last_exit_status(int status)
{
	g_last_exit_status = status;
}

int	get_last_exit_status(void)
{
	return (g_last_exit_status);
}
