/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:11:23 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 13:13:14 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Create a new list node.
 *
 * @param content The content to be added to the new node.
 * @return The newly allocated list node, or NULL if allocation fails.
 *
 * @details Allocates a new list node with the given content.
 * If 'content' is NULL,
 * initializes the 'content' pointer in the new node to NULL.
 */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list		*head;

	head = (t_list *)malloc(sizeof(t_list));
	if (!head)
		return (NULL);
	head -> content = content;
	head -> next = NULL;
	return (head);
}

// int main()
// {
// 	int value1 = 45;
// 	int value2 = 52;
// 	int value3 = 78;
// 	t_list	*node1 = ft_lstnew(&value1);
// 	t_list	*node2 = ft_lstnew(&value2);
// 	t_list	*node3 = ft_lstnew(&value3);
// 	t_list	*current;

// 	node1 -> next = node2;
// 	node2 -> next = node3;
// 	current = node1;
// 	while (current != NULL)
// 	{
// 		printf("The linked list is %d\n", *(int *)current -> content);
// 		current = current -> next;
// 	}
// 	current = node1;
// 	while (current != NULL)
// 	{
// 		t_list *temp = current;
// 		current = current -> next;
// 		free(temp);
// 	}
// }