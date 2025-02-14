#include "shell.h"


void signal_handler(int sig) {
    if (sig == SIGINT) {
        if (g_shell.in_heredoc) {
            write(STDOUT_FILENO, "\n", 1);
            g_shell.in_heredoc = 0;
            g_shell.signal_received = 1;
            close(STDIN_FILENO);
        } else if (g_shell.command_running) {
            write(STDOUT_FILENO, "\n", 1);
            g_shell.last_exit_status = 130;
            g_shell.signal_received = 1;
        } else {
            write(STDOUT_FILENO, "\n", 1);
            rl_on_new_line();
            rl_replace_line("", 0);
            rl_redisplay();
            g_shell.last_exit_status = 130;
        }
    }
}

void setup_signals(void) {
    struct sigaction sa = {
        .sa_handler = signal_handler,
        .sa_flags = SA_RESTART | SA_SIGINFO
    };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}