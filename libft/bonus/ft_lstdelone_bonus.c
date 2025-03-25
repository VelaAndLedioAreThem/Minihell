/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 23:45:39 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 13:24:12 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Delete a node and free its content using a given function.
 *
 * @param lst The node to be deleted.
 * @param del The function used to delete the content of the node.
 *
 * @details Deletes the node 'lst' and frees its content
 * using the function 'del'.
 * The memory of 'lst' itself is also freed. 
 * Does nothing if 'lst' or 'del' is NULL.
 */

#include "libft.h"

/**void	del(void *ptr)
{
	free(ptr);
}*/

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst == NULL)
		return ;
	if (del != NULL)
		del(lst -> content);
	free(lst);
}

/**int main()
{
    t_list            *new;
    int                *value1;
    int                *value2;
    int                *value3;

    value1 = (int *)malloc(sizeof(int));
    if (value1 == NULL)
        return (1);
    *value1 = 45;
    new = ft_lstnew(value1);
    if (new == NULL)
        return (1);
    value2 = (int *)malloc(sizeof(int));
    if (value2 == NULL)
        return (1);
    *value2 = 56;
    t_list *new_node = ft_lstnew(value2);
    new -> next = new_node;
    value3 = (int *)malloc(sizeof(int));
    if (value3 == NULL)
        return (1);
    *value3 = 20;
    t_list *last_node = ft_lstnew(value3);
	if (last_node == NULL)
		return (1);
	new -> next -> next = last_node;
	
	t_list *current = new;
	while (current != NULL)
	{
		printf("The current nodes are %d\n", *(int *)current -> content);
		current = current -> next;
	}
	current = new;
	while (current -> next != last_node)
	{
		current = current -> next;
	}
	current -> next = NULL;
	ft_lstdelone(last_node, del);
	printf("After deletion of the last nodes:\n");
	current = new;
	while (current != NULL)
	{
		printf("The nodes are %d\n", *(int *)current -> content);
		current = current -> next;
	}
	current = new;
	while (current != NULL)
	{
		t_list *temp = current -> next;
		ft_lstdelone(current, del);
		current = temp;
	}
}*/