#include "builtins.h"




// Handles environment-related commands: export, unset, env
int handle_env_commands(char **args) {
    if (!my_strcmp(args[0], "export")) {
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
    }
    return 0;
}

// Handles directory-related commands: cd, pwd
int handle_directory_commands(char **args) {
    if (!my_strcmp(args[0], "cd")) {
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
    }
    return 0;
}

// Handles remaining commands: echo, exit
int handle_other_commands(char **args) {
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
    } else if (!my_strcmp(args[0], "exit")) {
        printf("Running exit\n");
        exit(0);
    }
    return 0;
}
int handle_builtin(char **args) {
    if (handle_env_commands(args)) return 1;
    if (handle_directory_commands(args)) return 1;
    if (handle_other_commands(args)) return 1;
    return 0;
}