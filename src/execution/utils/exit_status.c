/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vela <vela@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:29:34 by vela              #+#    #+#             */
/*   Updated: 2025/06/06 17:30:10 by vela             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_last_exit_status(int status)
{
	g_last_exit_status = status;
}

int	get_last_exit_status(void)
{
	return (g_last_exit_status);
}
