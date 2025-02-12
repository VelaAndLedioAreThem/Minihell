#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

extern char **environ;
extern volatile sig_atomic_t g_signal;

/* signals.c prototypes */
void signal_handler(int sig);
void setup_signals(void);

/* envs.c prototypes */
int my_setenv(const char *name, const char *value, int overwrite);
int my_unsetenv(const char *name);

/* builtins.c prototypes */
int my_strcmp(const char *s1, const char *s2);
char **parse_command(char *input);
void free_tokens(char **tokens);
void execute_command(char **args);
int handle_builtin(char **args);
void handle_redirections(char **args);
void execute_pipeline(char **commands[], int num_commands);

#endif /* SHELL_H */
