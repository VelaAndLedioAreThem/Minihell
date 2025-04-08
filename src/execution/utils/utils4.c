
#include "../../include/minishell.h"

void *ft_realloc(void *ptr, size_t oldsize, size_t newsize) {
    char *newptr = malloc(newsize);
    if (!newptr) return NULL;
    
    if (ptr) {
        ft_memcpy(newptr, ptr, oldsize); // Copy from OLD to NEW
        free(ptr);
    }
    return newptr;
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	len;
	size_t	i;

	len = 0;
	while (len < n && s[len])
		len++;
	dup = (char *)malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[len] = '\0';
	return (dup);
}
