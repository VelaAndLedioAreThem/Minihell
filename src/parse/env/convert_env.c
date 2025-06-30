/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 20:29:25 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/30 15:59:08 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	is_after_heredoc(char *input, int pos)
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

t_env	*create_copy_env_node(t_env *original)
{
	t_env	*env_node;

	env_node = malloc(sizeof(t_env));
	if (!env_node)
		return (NULL);
	env_node->key = NULL;
	env_node->next = NULL;
	env_node->value = NULL;
	if (original->key)
	{
		env_node->key = ft_strdup(original->key);
		if (!env_node->key)
		{
			free(env_node);
			return (NULL);
		}
	}
	if (original->value)
	{
		env_node->value = ft_strdup(original->value);
		if (!env_node->value)
			return (free(env_node->key), free(env_node), NULL);
	}
	return (env_node);
}

void	process_env_var(t_args *parse, t_env *env_list, char *input)
{
	int		old_i;
	int		j;
	int		bslash_count;
	char	*tmp;

	j = parse->i;
	bslash_count = 0;
	while (input[j] == '\\')
	{
		j++;
		bslash_count++;
	}
	if (bslash_count && input[j] == '$')
	{
		if (parse->i > parse->start)
		{
			parse->temp = ft_substr(input, parse->start, parse->i
					- parse->start);
			parse->old_result = parse->result;
			parse->result = ft_strjoin(parse->result, parse->temp);
			free(parse->old_result);
			free(parse->temp);
		}
		tmp = ft_calloc(bslash_count / 2 + 1, sizeof(char));
		if (!tmp)
		{
			parse->i = j;
			return ;
		}
		ft_memset(tmp, '\\', bslash_count / 2);
		parse->old_result = parse->result;
		parse->result = ft_strjoin(parse->result, tmp);
		free(parse->old_result);
		free(tmp);
		if (bslash_count % 2)
		{
			parse->old_result = parse->result;
			parse->result = ft_strjoin(parse->result, "$");
			free(parse->old_result);
			parse->i = j + 1;
			parse->start = parse->i;
			return ;
		}
		parse->i = j;
		parse->start = parse->i;
	}
	if (quotes(input, parse->i, parse))
	{
		parse->i++;
		return ;
	}
	if (input[parse->i] == '$' && input[parse->i + 1] && input[parse->i
		+ 1] != '\'' && input[parse->i + 1] != ' ' && input[parse->i
		+ 1] != '"')
	{
		if (is_after_heredoc(input, parse->i))
		{
			parse->i++;
			return ;
		}
		old_i = parse->i;
		parse->result = handle_env_part(parse, &parse->i, env_list);
		if (parse->i == old_i)
			parse->i++;
		parse->start = parse->i;
	}
	else
		parse->i++;
}

t_env	*deep_copy_env_list(t_env *env_list)
{
	t_env	*new_head;
	t_env	*new_curr;
	t_env	*new_node;
	t_env	*original;

	new_head = NULL;
	new_curr = NULL;
	original = env_list;
	while (original)
	{
		new_node = create_copy_env_node(original);
		if (!new_node)
		{
			free_env_list(new_head);
			return (NULL);
		}
		if (!new_head)
			new_head = new_node;
		else
			new_curr->next = new_node;
		new_curr = new_node;
		original = original->next;
	}
	return (new_head);
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
