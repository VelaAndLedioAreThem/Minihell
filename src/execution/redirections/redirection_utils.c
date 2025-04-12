/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 12:42:00 by user              #+#    #+#             */
/*   Updated: 2025/04/07 15:39:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	print_error(char *filename, char *error_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(error_msg, STDERR_FILENO);
}

int	open_regular_input(char *last_red_inp)
{
	int	fd;

	fd = open(last_red_inp, O_RDONLY);
	if (fd < 0)
	{
		print_error(last_red_inp, strerror(errno));
		return (-1);
	}
	return (fd);
}
/* ==================== INPUT FILE HANDLING ==================== */

void	handle_redir_in(t_ast *node, char **last_red_inp)
{
	if (node->type == AST_REDIR_IN)
		*last_red_inp = node->token->value;
}

int	hh(t_ast *node, t_ast *data, char **delim, int *fd)
{
	if (node->type == AST_EOF)
	{
		*delim = node->token->value;
		*fd = create_heredoc_temp_file(*delim, data);
		if (*fd < 0)
			return (-1);
	}
	return (0);
}
