#include "shell.h"

int main(void) {
    setup_signals();
    printf("Starting minishell main loop\n");
    
    while (1) {
        char *input = readline("minishell$ ");
        if (!input) {
            printf("exit\n");
            break;
        }
        add_history(input);
        
        char **args = parse_command(input);
        if (!args || !args[0]) {
            free(input);
            free_tokens(args);
            continue;
        }
        
        handle_redirections(args);
        if (!handle_builtin(args)) {
            pid_t pid = fork();
            if (pid == -1)
                perror("fork");
            else if (pid == 0) {
                execute_command(args);
                exit(EXIT_FAILURE);
            } else {
                wait(NULL);
            }
        }
        free(input);
        free_tokens(args);
    }
    return 0;
}
