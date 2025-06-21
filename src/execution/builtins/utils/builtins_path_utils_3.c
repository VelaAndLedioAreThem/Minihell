/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_path_utils_3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:39:44 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/21 15:41:00 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_too_many_args(t_ast *data)
{
	ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
	data->exit_status = 1;
	return (1);
}
