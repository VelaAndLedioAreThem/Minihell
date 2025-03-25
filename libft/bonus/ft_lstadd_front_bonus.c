/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 14:43:06 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 12:19:14 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Add a new node with the specified content at the beginning of a list.
 *
 * @param lst A pointer to the pointer to the first node of the list.
 * @param new The new node to add to the beginning of the list.
 *
 * @details Adds the new node 'new' with the specified content to the beginning
 * of the list pointed to by 'lst'. Updates 'lst' to point to the new first node.
 */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst == NULL || new == NULL)
		return ;
	new -> next = *lst;
	*lst = new;
}
// int main()
// {
// 	t_list		*list;
// 	int			*value1;
// 	int			*value2;
// 	int			*value3;
// 	value1 = (int *)malloc(sizeof(int));
// 	if (value1 == NULL)
// 		return (1);
// 	*value1 = 45;
// 	list = ft_lstnew(value1);
// 	if (list == NULL)
// 		return (1);
// 	value2 = (int *)malloc(sizeof(int));
// 	if (value2 == NULL)
// 		return (1);
// 	*value2 = 30;
// 	t_list	*ptr;
// 	ptr = ft_lstnew(value2);
// 	if (ptr == NULL)
// 		return (1);
// 	list -> next = ptr;
// 	value3 = (int *)malloc(sizeof(int));
// 	if (value3 == NULL)
// 		return (1);
// 	*value3 = 20;
// 	t_list *new_node = ft_lstnew(value3);
// 	ft_lstadd_front(&list, new_node);
// 	while (list != NULL)
// 	{
// 		printf("%d\n", *(int *)list -> content);
// 		list = list -> next;
// 	}
// 	free(list);
// 	free(value1);
// 	free(ptr);
// 	free(value2);
// 	free(new_node);
// 	free(value3);
// }