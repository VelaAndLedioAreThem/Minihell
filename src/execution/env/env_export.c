/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/00/00 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/06/02 13:11:48 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	update_env_variable(t_env *env, char *name, char *eq)
{
	if (eq)
	{
		free(env->value);
		env->value = ft_strdup(eq + 1);
	}
	free(name);
}

static int	handle_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (1);
}

static int	process_export_arg(t_ast *data, char *arg)
{
  char	*eq;
  char	*name;
  t_env	*env;

  eq = ft_strchr(arg, '=');
  if (eq)
	name = ft_substr(arg, 0, eq - arg);
  else
	name = ft_strdup(arg);
  if (!name || !is_valid_identifier(name))
	return (free(name), handle_export_error(arg));
  env = get_env_node(data->env_list, name);
  if (env)
	update_env_variable(env, name, eq);
  else if (create_new_env(data, name, eq))
	return (1);
  return (0);
}

int	execute_export(t_ast *data, t_ast *tree, int fd_out)
{
	int		i;
	int		status;

	(void)fd_out;
	i = 1;
	status = 0;
	while (tree->cmd->args[i])
	{
		if (process_export_arg(data, tree->cmd->args[i]))
			status = 1;
		i++;
	}
	return (status);
}
