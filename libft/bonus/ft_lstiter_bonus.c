/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 12:05:33 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 12:20:12 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Iterate over each node of a list and apply a function to its content.
 *
 * @param lst The beginning of the list.
 * @param f The function to apply to the content of each node.
 *
 * @details Iterates over each node of the list starting from 'lst' and applies
 * the function 'f' to the content of each node.
 */

#include "libft.h"

/**void	print_content(void *ptr)
{
	int *value = (int *)ptr;
	(*value)++;
}*/

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (lst == NULL || f == NULL)
		return ;
	while (lst != NULL)
	{
		f(lst -> content);
		lst = lst -> next;
	}
}

/**int main()
{
	t_list		*list;
	int			*value1 = (int *)malloc(sizeof(int));
	int			*value2 = (int *)malloc(sizeof(int));
	
	if (!value1 || !value2)
		return (1);
	*value1 = 56;
	list = ft_lstnew(value1);
	*value2 = 78;
	t_list *new_node = ft_lstnew(value2);
	list -> next = new_node;
	ft_lstiter(list, print_content);
	while (list != NULL)
	{
		printf("The nodes are %d\n", *(int *)list -> content);
		list = list -> next;
	}
}*/