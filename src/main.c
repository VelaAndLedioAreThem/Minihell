#include "shell.h"

t_shell g_shell = {0};

int main(void) {
    setup_signals();
    printf("Starting minishell main loop\n");

    while (1) {
        // Reset signal state
        if (g_shell.signal_received) {
            g_shell.last_exit_status = 130;
            g_shell.signal_received = 0;
        }

        // Get current directory for prompt
        char cwd[128];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd");
            strcpy(cwd, "unknown");
        }

        // Build prompt
        char prompt[1024];
        snprintf(prompt, sizeof(prompt), "minishell:~%s$ ", cwd);

        // Read input
        char *input = readline(prompt);
        if (!input) { // Handle Ctrl-D
            printf("exit\n");
            break;
        }

        // Add to history if non-empty
        if (strlen(input) > 0) {
            add_history(input);
        }

        // Parse command into arguments
        char **args = parse_command(input);
        if (!args || !args[0]) {
            free(input);
            free_tokens(args);
            continue;
        }

        
        if (!handle_builtin(args)) {
            g_shell.command_running = 1; 
            pid_t pid = fork();
            if (pid == 0) { 
                handle_redirections(args);
                execute_command(args);
            } else { 
                int status;
                waitpid(pid, &status, 0);
                if (WIFEXITED(status)) {
                    g_shell.last_exit_status = WEXITSTATUS(status);
                }
                g_shell.command_running = 0;
            }
        }

        // Cleanup
        free(input);
        free_tokens(args);
    }
    return 0;
}