#include "shell.h"

void free_tokens(char **tokens) {
    int i = 0;
    while (tokens[i]) {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}