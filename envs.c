#include "shell.h"


extern char **environ;

int my_setenv(const char *name, const char *value, int overwrite) {
    if (!name || !*name || strchr(name, '=')) {
        write(2, "my_setenv: invalid name\n", 24);
        return -1;
    }
    size_t nlen = strlen(name);
    char *new_var = NULL;
    int i = 0;
    while (environ[i] != NULL) {
        if (strncmp(environ[i], name, nlen) == 0 && environ[i][nlen] == '=') {
            if (!overwrite)
                return 0;
            new_var = malloc(nlen + strlen(value) + 2);
            if (!new_var) { 
                perror("malloc");
                return -1;
            }
            sprintf(new_var, "%s=%s", name, value);
            free(environ[i]);
            environ[i] = new_var;
            return 0;
        }
        i++;
    }
    int count = i;
    new_var = malloc(nlen + strlen(value) + 2);
    if (!new_var) { 
        perror("malloc");
        return -1;
    }
    sprintf(new_var, "%s=%s", name, value);
    char **new_environ = malloc(sizeof(char*) * (count + 2));
    if (!new_environ) { 
        free(new_var);
        perror("malloc");
        return -1;
    }
    
    i = 0;
    while (i < count) {
        new_environ[i] = environ[i];
        i++;
    }
    
    new_environ[count] = new_var;
    new_environ[count + 1] = NULL;
    environ = new_environ;
    return 0;
}

int my_unsetenv(const char *name) {
    if (!name || !*name) {
        write(2, "my_unsetenv: invalid name\n", 26);
        return -1;
    }
    size_t name_len = strlen(name);
    int found = 0;
    int i = 0;
    while (environ[i] != NULL) {
        if (strncmp(environ[i], name, name_len) == 0 && environ[i][name_len] == '=') {
            free(environ[i]);
            int j = i;
            while (environ[j] != NULL) {
                environ[j] = environ[j+1];
                j++;
            }
            found = 1;
            continue;
        }
        i++;
    }
    return found ? 0 : -1;
}
