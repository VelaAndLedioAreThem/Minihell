/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: you <you@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:12:00 by you               #+#    #+#             */
/*   Updated: 2025/06/05 12:12:00 by you              ###   ########.fr       */
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

	new = val ? new_node(key, val) : new_node(key, NULL);
	if (!new)
		return (perror("malloc"), 1);
	if (add_env_node(&data->env_list, new))
	{
		free(new->key);
		free(new->value);
		free(new);
		return (perror("malloc"), 1);
	}
	return (0);
}

/* returns 0 success, 1 error */
int	export_one(t_ast *data, const char *arg)
{
	char	*key;
	char	*val;
	int		split_res;

	split_res = split_key_value(arg, &key, &val);
	if (split_res == 1 || (!is_valid_identifier(split_res == -1 ? arg : key)))
		return (ft_putendl_fd("export: not a valid identifier", 2),
				free(key), free(val), 1);
	if (split_res == -1)
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
		return (print_export_list(data->env_list, fd), 0);
	i = 1;
	ret = 1;
	while (args[i])
	{
		if (export_one(data, args[i]))
			ret = 0;
		++i;
	}
	return (ret);
}