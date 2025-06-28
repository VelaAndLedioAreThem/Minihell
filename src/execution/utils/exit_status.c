/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:35:47 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/28 17:35:49 by vszpiech         ###   ########.fr       */
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
