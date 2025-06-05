/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/06/05 00:00:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** --------------------------------------------------------------------------
**  export_print
**  -------------------------------------------------------------------------
**  Print every entry in the env list in KEY=VALUE form.
*/
static void	export_print(t_env *env, int fd_out)
{
	while (env)
	{
		ft_putstr_fd(env->key, fd_out);
		ft_putstr_fd("=", fd_out);
		env->value ? ft_putendl_fd(env->value, fd_out) : ft_putchar_fd('\n', fd_out);
		env = env->next;
	}
}

/*
** --------------------------------------------------------------------------
**  export_valid
**  -------------------------------------------------------------------------
**  Check the identifier for validity (first char alpha/_ , rest alnum/_).
*/
static int	export_valid(char *name)
{
	int	idx;

	idx = 0;
	if (!name || ft_isdigit(name[0]) || name[0] == '=')
		return (0);
	while (name[idx])
	{
		if (!ft_isalnum(name[idx]) && name[idx] != '_')
			return (0);
		idx++;
	}
	return (1);
}

/*
** --------------------------------------------------------------------------
**  export_set
**  -------------------------------------------------------------------------
**  Update an existing variable if found; otherwise create a new one.
*/
static int	export_set(t_ast *data, char *name, char *eq)
{
	t_env	*node;

	node = get_env_node(data->env_list, name);
	if (node && eq)
	{
		free(node->value);
		node->value = ft_strdup(eq + 1);
	}
	else if (!node)
	{
		if (create_new_env(data, name, eq))
			return (1);
	}
	return (0);
}

/*
** --------------------------------------------------------------------------
**  builtin_export
**  -------------------------------------------------------------------------
**  Main entry point used by handle_builtin().
*/
/* builtin_export.c */

int	builtin_export(t_ast *data, t_ast *tree, int fd_out)
{
	int		idx;
	int		status;
	char	*eq;
	char	*name;

	if (!tree->cmd->args[1])
	{
		export_print(data->env_list, fd_out);
		return (1);
	}
	idx = 1;
	status = 0;
	while (tree->cmd->args[idx])
	{
		eq   = ft_strchr(tree->cmd->args[idx], '=');
		name = eq ? ft_substr(tree->cmd->args[idx], 0, eq - tree->cmd->args[idx])
				: ft_strdup(tree->cmd->args[idx]);

		if (!export_valid(name))                 // invalid identifier
		{
			status = 1;
			free(name);                          // we bail out, so *now* we free
		}
		else if (export_set(data, name, eq))     // export_set takes ownership
			status = 1;

		idx++;
	}
	data->exit_status = status;
	return (1);
}

