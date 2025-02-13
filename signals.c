/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:24:18 by vszpiech          #+#    #+#             */
/*   Updated: 2025/02/13 17:24:18 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// signals.c
#include "shell.h"


void signal_handler(int sig)
{
    if (sig == SIGINT) {
        if (g_shell.in_heredoc) {
            write(STDOUT_FILENO, "\n", 1);
            g_shell.in_heredoc = 0; // Reset heredoc flag
            g_shell.signal_received = 1; // Set termination flag
            close(STDIN_FILENO); // Force exit from readline()
        } else {
            write(STDOUT_FILENO, "\n", 1);
            rl_replace_line("", 0);
            rl_on_new_line();
            rl_redisplay();
            g_shell.last_exit_status = 130;
        }
    }
}





void setup_signals(void)
{
    struct sigaction sa = {
        .sa_handler = signal_handler,
        .sa_flags = SA_RESTART
    };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}

