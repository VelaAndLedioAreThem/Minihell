/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 14:07:15 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 13:12:55 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Count the number of elements in a list.
 *
 * @param lst The beginning of the list.
 * @return The number of elements in the list.
 *
 * @details Counts and returns the number of nodes in
 * the list starting from 'lst'.
 */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int			count;

	count = 0;
	while (lst != NULL)
	{
		count++;
		lst = lst -> next;
	}
	return (count);
}

/**int main()
{
	t_list      *list;
	int         *value1;
	int         *value2;
	int			count;

	count = 0;
	value1 = (int *)malloc(sizeof(int));
	if (value1 == NULL)
		return (1);
	*value1 = 20;
	list = ft_lstnew(value1);

	value2 = (int *)malloc(sizeof(int));
	if (value2 == NULL)
		return (1);
	t_list *new = ft_lstnew(value2);
	list -> next = new;
	int length = ft_lstsize(list);
	printf("The number of nodes is %d\n", length);
	free(value1);
	free(list);
	free(value2);
	free(new);
}*/