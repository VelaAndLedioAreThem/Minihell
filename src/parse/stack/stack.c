/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 01:07:35 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/04 16:45:17 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	initialize_stack(t_token *stack)
{
	if (stack)
		stack->top = NULL;
}

bool	is_empty(t_token *stack)
{
	return (!stack || !stack->top);
}

char	pop(t_token *stack)
{
	char		data;
	t_stack		*temp;

	if (!stack || !stack->top)
		return ('\0');
	temp = stack->top;
	data = temp->name;
	stack->top = temp->next;
	free(temp);
	return (data);
}

void	free_stack(t_token *stack)
{
	t_stack		*current;
	t_stack		*next;

	if (!stack || !stack->top)
		return ;
	current = stack->top;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	stack->top = NULL;
}

void	push(t_token *stack, char data)
{
	t_stack		*new_node;

	new_node = (t_stack *)malloc(sizeof(t_stack));
	if (!new_node)
		return ;
	new_node->name = data;
	new_node->next = stack->top;
	stack->top = new_node;
}
