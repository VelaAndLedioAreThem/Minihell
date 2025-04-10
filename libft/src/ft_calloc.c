/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish <ldurmish@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 00:36:01 by ldurmish          #+#    #+#             */
/*   Updated: 2024/06/19 13:28:59 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ft_calloc - Allocate and zero-initialize array.
 * @count: The number of elements to be allocated.
 * @size: The size of each element.
 *
 * Description: This function allocates memory for an array of 'count'
 * elements of 'size' bytes each and initializes all its bits to zero.
 * The memory is dynamically allocated and needs to be freed by the caller.
 *
 * Return: A pointer to the allocated memory, or NULL if the allocation fails.
 */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t		i;
	char		*ptr;
	size_t		total_size;

	total_size = count * size;
	i = 0;
	ptr = malloc(total_size);
	if (count != 0 && total_size / count != size)
		return (NULL);
	if (!ptr)
		return (NULL);
	while (i < total_size)
	{
		ptr[i] = 0;
		i++;
	}
	return (ptr);
}

/**int main() {
    int num_elements = 5;
    int *arr;

    // Allocate memory for an array of integers using ft_calloc
    arr = (int *)ft_calloc(num_elements, sizeof(int));
    if (arr == NULL)
        return 1;
    // Print the values (should be initialized to 0)
    printf("Memory allocated by ft_calloc:\n");
    for (int i = 0; i < num_elements; i++)
        printf("%d ", arr[i]);
    printf("\n");
    free(arr);
    return 0;
}
*/