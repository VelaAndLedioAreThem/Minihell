#include "minishell.h"

/*
 * is_only_asterisks - Checks if the given string consists solely of '*' characters.
 * Returns 1 if true, 0 otherwise.
 */
int is_only_asterisks(char *str)
{
    int i = 0;
    if (!str)
        return 0;
    while (str[i])
    {
        if (str[i] != '*')
            return 0;
        i++;
    }
    return 1;
}
