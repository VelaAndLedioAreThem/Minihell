#include "shell.h"

volatile sig_atomic_t g_signal = 0;

void signal_handler(int sig) {
    g_signal = sig;
    char buf[128];
    snprintf(buf, sizeof(buf), "signal_handler called with signal: %d\n", sig);
    write(1, buf, strlen(buf));
    if (sig == SIGINT)
        write(1, "\nminishell$ ", 12);
}

void setup_signals(void) {
    printf("setup_signals called\n");
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, SIG_IGN);
}
