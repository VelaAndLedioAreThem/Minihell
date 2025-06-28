/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 20:29:25 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/22 15:36:47 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void     append_segment(t_args *p, int end)
{
       char    *tmp;
       char    *new_res;

       if (end > p->start)
       {
               tmp = ft_substr(p->input, p->start, end - p->start);
               if (!tmp)
                       return ;
               new_res = ft_strjoin(p->result, tmp);
               free(tmp);
               if (!new_res)
               {
                       free(p->result);
                       p->result = NULL;
                       return ;
               }
               free(p->result);
               p->result = new_res;
       }
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


void    process_env_var(t_args *parse, t_env *env_list, char *input)
{
        int             old_i;

        if (!parse->in_heredoc && !parse->single_quotes && !parse->double_quotes
                && input[parse->i] == '<' && input[parse->i + 1] == '<')
        {
                append_segment(parse, parse->i);
                parse->start = parse->i;
                parse->in_heredoc = 1;
                parse->i += 2;
                return ;
        }
        if (parse->in_heredoc)
        {
                if (quotes(input, parse->i, parse))
                {
                        parse->i++;
                        return ;
                }
                if (!parse->single_quotes && !parse->double_quotes
                        && (ft_isspace(input[parse->i]) || is_operator(input[parse->i])
                                || input[parse->i] == '\0'))
                {
                        append_segment(parse, parse->i);
                        parse->start = parse->i;
                        parse->in_heredoc = 0;
                        return ;
                }
                parse->i++;
                return ;
        }
        if (quotes(input, parse->i, parse))
        {
                parse->i++;
                return ;
        }
        if (input[parse->i] == '$' && input[parse->i + 1]
                && input[parse->i + 1] != '\'' && input[parse->i + 1] != ' '
                && input[parse->i + 1] != '"')
        {
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
