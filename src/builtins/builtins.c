#include "../../includes/shell.h"

int handle_builtin(char **args) {
    if (handle_env_commands(args)) return 1;
    if (handle_directory_commands(args)) return 1;
    if (handle_other_commands(args)) return 1;
    return 0;
}