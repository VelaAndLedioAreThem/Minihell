#include "minishell.h"


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
