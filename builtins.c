#include "shell.h"

int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}

char **parse_command(char *input) {
    printf("parse_command called with input: %s\n", input);
    int max_tokens = 64;
    char **tokens = malloc(max_tokens * sizeof(char *));
    if (!tokens) { perror("malloc"); exit(EXIT_FAILURE); }
    int i = 0;
    char *token = strtok(input, " ");
    while (token != NULL) {
        tokens[i++] = strdup(token);
        if (i >= max_tokens - 1)
            break;
        token = strtok(NULL, " ");
    }
    tokens[i] = NULL;
    return tokens;
}

void free_tokens(char **tokens) {
    printf("free_tokens called\n");
    for (int i = 0; tokens[i]; i++)
        free(tokens[i]);
    free(tokens);
}

void execute_command(char **args) {
    printf("Executing external command: %s\n", args[0]);
    if (strchr(args[0], '/') != NULL) {
        if (execve(args[0], args, environ) == -1) {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    } else {
        char *path_env = getenv("PATH");
        if (!path_env) {
            perror("getenv");
            exit(EXIT_FAILURE);
        }
        char *paths_copy = strdup(path_env);
        if (!paths_copy) { perror("strdup"); exit(EXIT_FAILURE); }
        char *token = strtok(paths_copy, ":");
        int found = 0;
        while (token != NULL) {
            int len = strlen(token) + 1 + strlen(args[0]) + 1;
            char *full_path = malloc(len);
            if (!full_path) { perror("malloc"); free(paths_copy); exit(EXIT_FAILURE); }
            snprintf(full_path, len, "%s/%s", token, args[0]);
            printf("Checking path: %s\n", full_path);
            if (access(full_path, X_OK) == 0) {
                printf("Found executable: %s\n", full_path);
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
            write(2, "Command not found\n", 18);
            exit(EXIT_FAILURE);
        }
    }
}

int handle_builtin(char **args) {
    printf("handle_builtin called with command: %s\n", args[0]);
    if (!my_strcmp(args[0], "echo")) {
        printf("Running echo\n");
        int newline = 1, i = 1;
        if (args[1] && !my_strcmp(args[1], "-n")) {
            newline = 0;
            i++;
        }
        for (; args[i]; i++) {
            printf("%s", args[i]);
            if (args[i+1])
                printf(" ");
        }
        if (newline)
            printf("\n");
        return 1;
    } else if (!my_strcmp(args[0], "cd")) {
        printf("Running cd\n");
        if (!args[1])
            fprintf(stderr, "cd: missing argument\n");
        else if (chdir(args[1]) != 0)
            perror("cd");
        return 1;
    } else if (!my_strcmp(args[0], "pwd")) {
        printf("Running pwd\n");
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)))
            printf("%s\n", cwd);
        else
            perror("pwd");
        return 1;
    } else if (!my_strcmp(args[0], "export")) {
        printf("Running export\n");
        if (!args[1]) {
            printf("Listing exported variables:\n");
            for (char **env = environ; *env; env++)
                printf("%s\n", *env);
        } else {
            for (int i = 1; args[i]; i++) {
                char *equal_sign = strchr(args[i], '=');
                if (equal_sign) {
                    int name_len = equal_sign - args[i];
                    char *varname = malloc(name_len + 1);
                    if (!varname) { perror("malloc"); exit(EXIT_FAILURE); }
                    strncpy(varname, args[i], name_len);
                    varname[name_len] = '\0';
                    int ret = my_setenv(varname, equal_sign + 1, 1);
                    if (ret == 0)
                        printf("Exported: %s\n", args[i]);
                    else
                        printf("Failed to export: %s\n", args[i]);
                    free(varname);
                } else {
                    char *val = getenv(args[i]);
                    if (val)
                        printf("Variable %s already exported with value: %s\n", args[i], val);
                    else
                        printf("No value to export for: %s\n", args[i]);
                }
            }
        }
        return 1;
    } else if (!my_strcmp(args[0], "unset")) {
        printf("Running unset\n");
        if (!args[1])
            fprintf(stderr, "unset: not enough arguments\n");
        else {
            for (int i = 1; args[i]; i++) {
                int ret = my_unsetenv(args[i]);
                if (ret == 0)
                    printf("Unset: %s\n", args[i]);
                else
                    printf("Failed to unset: %s\n", args[i]);
            }
        }
        return 1;
    } else if (!my_strcmp(args[0], "env")) {
        printf("Running env\n");
        for (char **env = environ; *env; env++)
            printf("%s\n", *env);
        return 1;
    } else if (!my_strcmp(args[0], "exit")) {
        printf("Running exit\n");
        exit(0);
    }
    return 0;
}

void handle_redirections(char **args) {
    printf("handle_redirections called\n");
    for (int i = 0; args[i]; i++) {
        if (!my_strcmp(args[i], ">")) {
            printf("Redirection: output to file: %s\n", args[i+1]);
            int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
            break;
        } else if (!my_strcmp(args[i], ">>")) {
            printf("Redirection: append output to file: %s\n", args[i+1]);
            int fd = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
            break;
        } else if (!my_strcmp(args[i], "<")) {
            printf("Redirection: input from file: %s\n", args[i+1]);
            int fd = open(args[i+1], O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;
            break;
        }
    }
}

void execute_pipeline(char **commands[], int num_commands) {
    printf("execute_pipeline called with %d commands\n", num_commands);
    int pipefd[2];
    pid_t pid;
    int in_fd = STDIN_FILENO;
    for (int i = 0; i < num_commands; i++) {
        pipe(pipefd);
        pid = fork();
        if (pid == -1) { perror("fork"); exit(EXIT_FAILURE); }
        if (pid == 0) {
            dup2(in_fd, STDIN_FILENO);
            if (i != num_commands - 1)
                dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
            handle_redirections(commands[i]);
            if (!handle_builtin(commands[i]))
                execute_command(commands[i]);
            exit(EXIT_SUCCESS);
        }
        close(pipefd[1]);
        in_fd = pipefd[0];
    }
    while (wait(NULL) > 0);
}
