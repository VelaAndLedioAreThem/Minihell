/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:04:11 by ldurmish          #+#    #+#             */
/*   Updated: 2025/03/12 20:27:27 by ldurmish         ###   ########.fr       */
/*   Updated: 2025/02/13 14:53:43 by ldurmish         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/include/libft.h"

// Parsing struct 
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_PAREN_OPEN,
	TOKEN_PAREN_CLOSE,
	TOKEN_EOF,
	TOKEN_WILDCARD,
}	t_token_type;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_quote
{
	bool			in_single_quotes;
	bool			in_double_quotes;
}	t_quotes;

// Parenthesis struct
typedef struct s_open_paren
{
	bool			has_commands;
	bool			expecting_commands;
	int				i;
}	t_open_paren;

typedef struct s_paren
{
	bool			has_commands;
	bool			has_content;
	char			last_op;
	bool			has_cmd_before;
	int				j;
	bool			has_operator;
	t_quotes		quote;
}	t_paren;

// Commands struct
typedef struct s_cmd
{
	bool		in_commands;
	bool		is_cmd_pos;
}	t_cmd;

typedef struct s_stack
{
	char			name;
	struct s_stack	*next;
}	t_stack;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				expandable;
	t_stack			*top;
	struct s_token	*next;
}	t_token;

typedef struct s_pipe
{
	t_token		*curr;
	t_token		*prev;
	bool		expecting_command;
}	t_pipe;

typedef struct s_nest
{
	bool			success;
	t_token			*token;
	int				position;
}	t_nest;

typedef struct s_state
{
	t_quotes		quote;
	t_token			*token;
	int				pos;
	int				paren_count;
}	t_state;

typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPELINE,
	AST_REDIR,
	AST_AND_OR,
	AST_SUBSHELL,
	AST_SEQUENCING
}	t_ast_type;

// Erorrs Messages
typedef enum e_errors_code
{
	ERR_NONE,
	ERR_UNBALANCED_PAREN,
	ERR_SYNTAX,
	ERR_UNEXPECTED_TOKEN,
	ERR_MEMORY,
	ERR_PARSE
}	t_errors_code;

typedef struct s_command
{
	char			**args;
	char			*infile;
	char			*outfile;
	char			*heredoc_delim;
	int				append;
	int				is_builtin;
}	t_commands;

typedef struct s_args
{
	int				argc;
	char			**argv;
	int				exit_status;
	char			*input;
	int				start;
	char			*result;
	int				single_quotes;
	int				double_quotes;
	int				last_quotes;
}	t_args;

// Main struct
typedef struct s_ast
{
	t_ast_type		type;
	struct s_ast	*left;
	struct s_ast	*right;
	t_commands		*cmd;
	int				operator_type;
	t_token			*token;
}	t_ast;

// Tokenization functions
t_token		*create_node(char *str, t_token_type type);
t_token		*tokenize(char *input);
void		append_node(t_token **token, t_token *current_token);
int			is_operator(char c);
int			return_parenthesis(t_token **token, char c);
int			handle_double_operator(t_token **head, char *input, int *i);
int			handle_single_operator(t_token **token, char c);
int			handle_word(t_token **token, char *input, int *i);
int			handle_quotes(t_token **token, char *input, int *i);

// Environmental variables
t_env		*init_env_list(char **envp);
char		*gen_env_value(t_env *env_list, char *key);
char		*parse_env(char *input, t_env *env_list, t_args *arg);
char		*join_arguments(t_args *arg);
char		*get_env_value(t_env *env_list, char *name);
char		*join_arguments(t_args *arg);
char		*env_expansion(char *input, int *i, t_env *env_list, t_args *arg);

// Validation
bool		validation(t_token *tokens);
bool		validation_parenthesis(t_token *tokenize);

// Parenthesis
bool		check_parenthesis(t_token *token, char *input, int i,
				t_paren *commands);
void		process_quotes(char c, t_quotes *quote);
bool		valid_before_open_paren(char c);
bool		is_valid_after_open_paren(char c);
int			find_matching_paren(t_token *token, char *input, int start_pos);
bool		validate_paren_content(char *input, int start_pos, int end_pos,
				t_token *token);
t_nest		validate_nested_paren(char *input, int start, int end,
				t_token *token);
bool		op_paren(char *input, t_open_paren *paren,
				t_token *token);
bool		pipe_paren(char *input, t_open_paren *paren,
				t_token *token);
bool		seq_paren(char *input, t_open_paren *paren,
				t_token *token);
bool		validate_redirect_in_paren(char *input, t_open_paren *paren,
				int end, t_token *token);
bool		validate_redirect_or_command(char *input, t_open_paren *paren,
				int end, t_token *token);
bool		validate_paren_content_utils(t_open_paren *paren, t_token *token);
bool		is_valid_close_paren(char c);
bool		process_close_paren(char *input, int i, t_token *token,
				t_paren *command);
bool		skip_whitespaces(char *input, int *i, int end);
bool		check_next_token(t_token *next);
bool		check_after_close_paren(char *input, int *i, t_token *token);
bool		it_is_log_or_pipe(char *input, int *i, t_token *token);
bool		it_is_logical_op(char *input, int *i, int *j, t_token *token);
bool		it_is_pipe(char *input, int *i, int *j, t_token *token);

// Operators
bool		ft_is_operator(char c);
bool		ft_is_logical_op(char current, char next);
bool		validate_logical_op(t_token *tokens);

// Quotes
bool		validation_quotes(t_token *tokens);

// Redirections
bool		ft_is_redirection(char c);
bool		ft_is_redirection_op(char current, char next);

//Commands
bool		ft_is_commands_position(char *input, int i);
bool		validate_commands(t_token *tokenize);
bool		is_valid_command_start(char c);
bool		is_valid_command_char(char c);
bool		is_command_or_arg_char(char c);

// Wildcards
bool		ft_is_wildcard(char c);

// Pipes
bool		pipes(t_token *tokenize);

// Stack operations
void		push(t_token *stack, char data);
void		initialize_stack(t_token *stack);
char		pop(t_token *stack);
void		free_stack(t_token *stack);
bool		is_empty(t_token *stack);

// Errors and Free functions
void		free_env_list(t_env *env_list);
t_token		*free_tokens(t_token *token);
void		report_error(t_errors_code code, char *token);
void		free_stack(t_token *token);

// Utils functions
int			ft_strcmp(char *s1, char *s2);
int			count_parenthesis(t_token *tokens);
int			ft_isspace(int num);
#endif
