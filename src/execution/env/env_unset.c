#include "../../../include/minishell.h"

static void	remove_env_node(t_ast *data, t_env *prev, t_env *curr)
{
	if (prev)
		prev->next = curr->next;
	else
		data->env_list = curr->next;
	free(curr->key);
	free(curr->value);
	free(curr);
}

static int	handle_unset_error(char *name)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (1);
}
static int	process_unset_arg(t_ast *data, char *name)
{
	t_env	*prev;
	t_env	*curr;

	if (!is_valid_identifier(name))
		return (handle_unset_error(name));
	prev = NULL;
	curr = data->env_list;
	while (curr)
	{
		if (!ft_strcmp(curr->key, name))
		{
			remove_env_node(data, prev, curr);
			break ;
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}

int	execute_unset(t_ast *data, t_ast *tree)
{
	int	i;

	i = 1;
	while (tree->cmd->args[i])
	{
		process_unset_arg(data, tree->cmd->args[i]);
		i++;
	}
	return (0);
}
