/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:10:00 by you               #+#    #+#             */
/*   Updated: 2025/06/30 13:28:00 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

/* --- tiny helpers -------------------------------------------------------- */

int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(*s) && *s != '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		++i;
	}
	return (1);
}

/* returns: 0 ok, 1 malloc fail, -1 no '=' */
int	split_key_value(const char *arg, char **key, char **val)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (-1);
	*key = ft_substr(arg, 0, eq - arg);
	if (!*key)
		return (1);
	*val = ft_strdup(eq + 1);
	if (!*val)
		return (free(*key), 1);
	return (0);
}

void	print_env_list(t_env *lst, int fd)
{
	while (lst)
	{
		if (lst->value)
		{
			ft_putstr_fd(lst->key, fd);
			ft_putstr_fd("=", fd);
			ft_putendl_fd(lst->value, fd);
		}
		lst = lst->next;
	}
}
