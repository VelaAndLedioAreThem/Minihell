/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 20:13:28 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 12:19:45 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Add a new node with the specified content at the end of a list.
 *
 * @param lst A pointer to the pointer to the first node of the list.
 * @param new The new node to add to the end of the list.
 *
 * @details Adds the new node 'new' with the specified content to the end
 * of the list pointed to by 'lst'. Updates 'lst' if it's initially NULL.
 */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list		*last;

	if (lst == NULL || new == NULL)
	{
		return ;
	}
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast(*lst);
	last -> next = new;
}
/**int main()
{
	t_list		*new;
	int			*value1;
	int			*value2;
	int			*value3;

	value1 = (int *)malloc(sizeof(int));
	if (value1 == NULL)
		return (1);
	*value1 = 54;
	new = ft_lstnew(value1);
	
	value2 = (int *)malloc(sizeof(int));
	if (value2 == NULL)
		return (1);
	*value2 = 56;
	t_list *new_node = ft_lstnew(value2);
	new -> next = new_node;

	value3 = (int *)malloc(sizeof(int));
	if (value3 == NULL)
		return (1);
	*value3 = 90;
	t_list *last_node = ft_lstnew(value3);
	if (last_node == NULL)
		return (1);
	
	ft_lstadd_back(&new, last_node);
	t_list *current = new;
	if (new != NULL)
	{
		while (current -> next != NULL)
		{
			current = current -> next;
		}
		printf("The last linked list is %d\n", *(int *)current-> content);
	}
	free(new);
	free(value1);
	free(value2);
	free(new_node);
	free(value3);
	free(last_node);
}*/