#include "shell.h"

char **parse_command(char *input) {
    int max_tokens = 64;
    char **tokens = malloc(max_tokens * sizeof(char *));
    if (!tokens) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    char *token = strtok(input, " ");
    while (token != NULL) {
        tokens[i++] = strdup(token);
        if (i >= max_tokens - 1) break;
        token = strtok(NULL, " ");
    }
    tokens[i] = NULL;

    int j = 0;
    while (tokens[j]) {
        if (my_strcmp(tokens[j], "$?") == 0) {
            free(tokens[j]);
            char exit_str[12];
            snprintf(exit_str, sizeof(exit_str), "%d", g_shell.last_exit_status);
            tokens[j] = strdup(exit_str);
        }
        j++;
    }

    return tokens;
}