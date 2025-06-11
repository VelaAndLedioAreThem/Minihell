/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 19:50:51 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/10 11:14:16 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	append_node(t_token **head, t_token *current_token)
{
	t_token		*temp;

	if (!head || !current_token)
		return ;
	if (*head == NULL)
	{
		*head = current_token;
		return ;
	}
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = current_token;
	}
}

t_token	*create_node(char *str, t_token_type type)
{
	t_token		*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->value = ft_strdup(str);
	if (!node->value)
	{
		free(node);
		return (NULL);
	}
	node->type = type;
	node->expandable = 0;
	node->quotes.in_double_quotes = false;
	node->quotes.in_single_quotes = false;
	node->next = NULL;
	return (node);
}
