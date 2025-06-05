/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_display.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourlogin <you@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:00:00 by yourlogin         #+#    #+#             */
/*   Updated: 2025/06/05 12:00:00 by yourlogin        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/* ---------- helpers ------------------------------------------------------ */

static int	count_nodes(t_env *lst)
{
	int	n;

	n = 0;
	while (lst)
	{
		n++;
		lst = lst->next;
	}
	return (n);
}

static char	**env_to_array(t_env *env, int *len)
{
	int		i;
	char	**array;

	*len = count_nodes(env);
	array = malloc(sizeof(char *) * (*len + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (env)
	{
		array[i++] = ft_strjoin3(env->key, "=", env->value ? env->value : "");
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}

static void	sort_ascii_array(char **arr, int len)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < len - 1)
	{
		j = i + 1;
		while (j < len)
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

/* ---------- public ------------------------------------------------------- */
/*
** Bash-compatible “declare -x” listing used by `export` with no arguments.
*/

void	print_export_list(t_env *env, int fd_out)
{
	int		len;
	char	**arr;
	char	*eq;
	int		i;

	if (!env)
		return ;
	arr = env_to_array(env, &len);
	if (!arr)
		return ;
	sort_ascii_array(arr, len);
	i = 0;
	while (i < len)
	{
		eq = ft_strchr(arr[i], '=');
		ft_putstr_fd("declare -x ", fd_out);
		if (eq)
		{
			*eq = '\0';
			ft_putstr_fd(arr[i], fd_out);
			ft_putstr_fd("=\"", fd_out);
			ft_putstr_fd(eq + 1, fd_out);
			ft_putendl_fd("\"", fd_out);
			*eq = '=';
		}
		else
			ft_putendl_fd(arr[i], fd_out);
		free(arr[i]);
		i++;
	}
	free(arr);
}
