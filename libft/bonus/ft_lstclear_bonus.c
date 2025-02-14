/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 13:07:29 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 13:23:50 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Delete all nodes of a list and their content using a given function.
 *
 * @param lst A pointer to the pointer to the first node of the list.
 * @param del The function used to delete the content of each node.
 *
 * @details Deletes all nodes of the list starting from
 * 'lst' and frees their content using the function 'del'. 
 * Updates 'lst' to NULL. Does nothing if 'lst' or 'del' is NULL.
 */

#include "libft.h"

/*void	del(void *ptr)
{
	free(ptr);
}*/

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list		*temp;

	if (*lst == NULL || del == NULL)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = temp;
	}
	*lst = NULL;
}

/**int main()
{
    t_list *list;
    int *value1 = (int *)malloc(sizeof(int));
    int *value2 = (int *)malloc(sizeof(int));
    int *value3 = (int *)malloc(sizeof(int));

    if (!value1 || !value2 || !value3)
        return 1;

    *value1 = 42;
    *value2 = 58;
    *value3 = 100;

    list = ft_lstnew(value1);
    if (list == NULL)
        return 1;

    t_list *new2 = ft_lstnew(value2);
    if (new2 == NULL)
        return 1;
    list->next = new2;
    t_list *current = list;
    while (current != NULL)
    {
        printf("Node value: %d\n", *(int *)(current->content));
        current = current->next;
    }

    // Clear the list
    ft_lstclear(&list, del);

    // Ensure the list is cleared
    if (list == NULL)
        printf("List has been cleared successfully.\n");
    else
        printf("List has not been cleared successfully.\n");

    return 0;
}*/