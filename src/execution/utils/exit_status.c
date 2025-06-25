/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:29:34 by vela              #+#    #+#             */
/*   Updated: 2025/06/25 16:45:27 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_last_exit_status(t_ctx *ctx, int status)
{
	if (ctx)
		ctx->last_exit_status = status;
}

int	get_last_exit_status(t_ctx *ctx)
{
	if (!ctx)
		return (0);
	return (ctx->last_exit_status);
}
