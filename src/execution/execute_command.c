#include "shell.h"

void execute_command(char **args) {
    char *path_env;
    char *paths_copy;
    char *token;
    int found;
    int len;
    char *full_path;

    if (strchr(args[0], '/') != NULL) {
        if (execve(args[0], args, environ) == -1) {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    } else {
        path_env = getenv("PATH");
        if (!path_env) {
            perror("getenv");
            exit(EXIT_FAILURE);
        }
        paths_copy = strdup(path_env);
        if (!paths_copy) {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
        token = strtok(paths_copy, ":");
        found = 0;
        while (token != NULL) {
            len = strlen(token) + 1 + strlen(args[0]) + 1;
            full_path = malloc(len);
            if (!full_path) {
                perror("malloc");
                free(paths_copy);
                exit(EXIT_FAILURE);
            }
            snprintf(full_path, len, "%s/%s", token, args[0]);
            if (access(full_path, X_OK) == 0) {
                found = 1;
                if (execve(full_path, args, environ) == -1) {
                    perror("execve");
                    free(full_path);
                    free(paths_copy);
                    exit(EXIT_FAILURE);
                }
            }
            free(full_path);
            token = strtok(NULL, ":");
        }
        free(paths_copy);
        if (!found) {
            write(2, "minishell: command not found: ", 31);
            write(2, args[0], strlen(args[0]));
            write(2, "\n", 1);
            g_shell.last_exit_status = 127; 
            exit(127);                    
        }
    }
}