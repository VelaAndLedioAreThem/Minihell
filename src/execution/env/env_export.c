/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:33:31 by vszpiech          #+#    #+#             */
/*   Updated: 2025/06/28 17:33:31 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	handle_existing(t_ast *data, char *key, char *val)
{
	if (val && update_env_value(data->env_list, key, val))
		return (perror("malloc"), 1);
	return (0);
}

static int	add_new(t_ast *data, char *key, char *val)
{
	t_env	*new;

	new = NULL;
	if (val == NULL)
		new = new_node(key, NULL);
	else
		new = new_node(key, val);
	if (new == NULL)
		return (perror("malloc"), 1);
	if (add_env_node(&data->env_list, new) != 0)
	{
		free(new->key);
		free(new->value);
		free(new);
		return (perror("malloc"), 1);
	}
	return (0);
}

int	export_one(t_ast *data, const char *arg)
{
	char	*key;
	char	*val;
	int		s;

	key = NULL;
	val = NULL;
	s = split_key_value(arg, &key, &val);
	if (s == 1
		|| (s == -1 && !is_valid_identifier(arg))
		|| (s != -1 && !is_valid_identifier(key)))
		return (ft_putendl_fd("export: not a valid identifier", 2),
			free(key), free(val), 1);
	if (s == -1)
	{
		key = ft_strdup(arg);
		if (!key)
			return (perror("malloc"), 1);
	}
	if (find_env_node(data->env_list, key))
		return (handle_existing(data, key, val), free(key), free(val), 0);
	add_new(data, key, val);
	return (free(key), free(val), 0);
}

static void	print_export_list(t_env *lst, int fd)
{
	while (lst)
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putstr_fd(lst->key, fd);
		if (lst->value)
		{
			ft_putstr_fd("=\"", fd);
			ft_putstr_fd(lst->value, fd);
			ft_putstr_fd("\"", fd);
		}
		ft_putchar_fd('\n', fd);
		lst = lst->next;
	}
}

int	builtin_export(t_ast *data, t_ast *tree, int fd)
{
	char	**args;
	int		i;
	int		ret;

	args = tree->cmd->args;
	if (!args[1])
	{
		print_export_list(data->env_list, fd);
		data->exit_status = 0;
		return (1);
	}
	i = 1;
	ret = 1;
	while (args[i])
	{
		if (export_one(data, args[i]))
			ret = 0;
		++i;
	}
	if (ret == 1)
		data->exit_status = 0;
	else
		data->exit_status = 1;
	return (1);
}
