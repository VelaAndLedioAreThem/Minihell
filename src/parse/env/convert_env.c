/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 20:29:25 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/30 16:52:28 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_after_heredoc(char *input, int pos)
{
	int	i;

	i = pos - 1;
	while (i >= 0 && ft_isspace(input[i]))
		i--;
	if (i >= 0 && (input[i] == '"' || input[i] == '\''))
		i--;
	while (i >= 0 && ft_isspace(input[i]))
		i--;
	if (i >= 1 && input[i] == '<' && input[i - 1] == '<')
		return (1);
	return (0);
}

int	append_backslashes(t_args *p, int n)
{
	char	*blk;
	char	*old;

	if (n <= 0)
		return (1);
	blk = ft_calloc(n + 1, sizeof(char));
	if (!blk)
		return (0);
	ft_memset(blk, '\\', n);
	old = p->result;
	p->result = ft_strjoin(p->result, blk);
	free(old);
	free(blk);
	return (1);
}

char	*handle_special_utils(char *input, char *str, int *i, t_args *arg)
{
	if (input[*i] == '?')
	{
		str = ft_itoa(arg->exit_status);
		(*i)++;
	}
	else if (input[*i] == '@' || input[*i] == '*')
	{
		str = join_arguments(arg);
		(*i)++;
	}
	else if (input[*i] == '0')
	{
		str = ft_strdup(arg->argv[0]);
		(*i)++;
	}
	return (str);
}

char	*get_env_value(t_env *env_list, char *name)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (!ft_strcmp(current->key, name))
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

t_env	*init_env_list(char **envp)
{
	t_env	*head;
	t_env	*current;
	int		i;
	t_env	*new_node;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = create_env_node(envp[i]);
		if (!new_node)
		{
			free_env_list(head);
			return (NULL);
		}
		if (!head)
			head = new_node;
		else
			current->next = new_node;
		current = new_node;
		i++;
	}
	return (head);
}
