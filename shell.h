/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vszpiech <vszpiech@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:24:08 by vszpiech          #+#    #+#             */
/*   Updated: 2025/02/13 17:24:08 by vszpiech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SHELL_H
# define SHELL_H

# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

extern char **environ;
extern volatile sig_atomic_t g_signal;
typedef struct s_shell {
    int         last_exit_status;
    volatile sig_atomic_t signal_received;
    int         in_heredoc;
} t_shell;

extern t_shell g_shell;



extern t_shell g_shell;
/* signals.c prototypes */
void	signal_handler(int sig);
void	setup_signals(void);
void	handle_redirections(char **args);
int		handle_builtin(char **args);
/* envs.c prototypes */
int	my_setenv(const char *name, const char *value, int overwrite);
int	my_unsetenv(const char *name);

/* builtins.c prototypes */
int	my_strcmp(const char *s1, const char *s2);
char	**parse_command(char *input);
void	free_tokens(char **tokens);
void	execute_command(char **args);
int	handle_builtin(char **args);
void	handle_redirections(char **args);
void	execute_pipeline(char **commands[], int num_commands);

#endif /* SHELL_H */
