/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:02:27 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 11:29:37 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** ft_lstmap:
**    Iterates the linked list 'lst' and applies the function 'f' to each
**    content. Creates a new list resulting from the applications of 'f'.
**    If the allocation fails, it cleans up the allocated memory.
**
**    lst: The original list to iterate.
**    f:   The function to apply to each content of the list.
**    del: The function to delete a single element's content.
**
** Returns:
**    A new list resulting from the applications of 'f', or NULL if the
**    allocation fails or any parameter is NULL.
*/

#include "libft.h"

// void	*increment(void *ptr)
// {
// 	int *value = (int *)ptr;
// 	(*value)++;
// 	return (value);
// }

// void	del_lstmap(void *ptr)
// {
// 	free(ptr);
// }

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list			*new_node;
	t_list			*new_list;
	void			*new_content;

	if (lst == NULL || f == NULL || del == NULL)
		return (NULL);
	new_list = NULL;
	while (lst != NULL)
	{
		new_content = f(lst -> content);
		new_node = ft_lstnew(new_content);
		if (!new_node)
		{
			del(new_content);
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_node);
		lst = lst -> next;
	}
	return (new_list);
}
// int main()
// {
// 	t_list		*list;
// 	int			*value1 = (int *)malloc(sizeof(int));
// 	int			*value2 = (int *)malloc(sizeof(int));

// 	if (!value1 || !value2)
// 		return (1);
// 	*value1 = 56;
// 	list = ft_lstnew(value1);
// 	*value2 = 67;
// 	t_list *new_node = ft_lstnew(value2);
// 	list -> next = new_node;
// 	t_list *current = list;
// 	printf("The original list:\n");
// 	while (current != NULL)
// 	{
// 		printf("The nodes are %d\n", *(int *)current -> content);
// 		current = current -> next;
// 	}
// 	printf("The new list is:\n");
// 	t_list	*new_list = ft_lstmap(list, increment, del_lstmap);
// 	t_list *result = new_list;
// 	while (result != NULL)
// 	{
// 		printf("The new list value are %d\n", *(int *)result -> content);
// 		result = result -> next;
// 	}
// 	ft_lstclear(&new_list, del_lstmap);
// 	ft_lstclear(&list, del_lstmap);
// }