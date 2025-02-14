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

typedef struct s_shell {
    int last_exit_status;
    volatile sig_atomic_t signal_received;
    int in_heredoc;
    int command_running; 
} t_shell;

extern t_shell g_shell;

/* signals.c prototypes */
void signal_handler(int sig);
void setup_signals(void);

/* envs.c prototypes */
int my_setenv(const char *name, const char *value, int overwrite);
int my_unsetenv(const char *name);

/* builtins/builtins.c prototypes */
int handle_builtin(char **args);
int handle_env_commands(char **args);
int handle_directory_commands(char **args);
int handle_other_commands(char **args) ;
/* parsing/parse_command.c prototypes */
char **parse_command(char *input);

/* execution/execute_command.c prototypes */
void execute_command(char **args);

/* execution/execute_pipeline.c prototypes */
void execute_pipeline(char **commands[], int num_commands);

/* execution/handle_redirections.c prototypes */
void handle_redirections(char **args);

/* utils/my_strcmp.c prototypes */
int my_strcmp(const char *s1, const char *s2);

/* utils/free_tokens.c prototypes */
void free_tokens(char **tokens);

/* utils/error_handling.c prototypes */
void handle_error(const char *msg);

#endif /* SHELL_H */