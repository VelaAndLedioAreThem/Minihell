/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vela <vela@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 23:21:28 by ldurmish          #+#    #+#             */
/*   Updated: 2025/06/16 00:39:27 by ldurmish         ###   ########.fr       */
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
# define TOKEN_PROCESSED 42
# define TOKEN_READWRITE 43
# define HEREDOC_TEMPLATE "/tmp/minishell_heredocXXXXXX"

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
	TOKEN_WHITESPACE
}	t_token_type;

typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPELINE,
	AST_REDIR_IN,
	AST_REDIR_OUT,
	AST_REDIR_APPEND,
	AST_REDIR_HERDOC,
	AST_AND,
	AST_OR,
	AST_SUBSHELL,
	AST_EOF
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

typedef enum e_flag_state
{
	FLAG_NONE,
	FLAG_SINGLE_DASH,
	FLAG_DOUBLE_DASH
}	t_flag_state;

typedef struct s_rm_quotes
{
	char			*result;
	int				i;
	int				j;
	int				in_quotes;
	char			quote_char;
}	t_rm_quotes;

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
	t_quotes		quotes;
	t_stack			*top;
	int				from_expansion;
	struct s_token	*next;
}	t_token;

typedef struct s_pipe
{
	t_token		*curr;
	t_token		*prev;
	t_token		*head;
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

typedef struct s_redir_list
{
	int					type;
	char				*filename;
	struct s_redir_list	*next;
}	t_redir_list;

typedef struct s_command
{
	char			**args;
	char			*infile;
	char			*outfile;
	char			*heredoc_delim;
	int				append;
	int				is_builtin;
	t_redir_list	*redirections;
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
	int				i;
}	t_args;

typedef struct s_expand_wild
{
	int				i;
	int				j;
	int				k;
	char			**expanded;
	char			**final_args;
	char			**temp_args;
	t_token			*curr;
	t_env			*env;
	t_args			*args;
}	t_expand_wild;

// Main struct
typedef struct s_ast
{
	t_ast_type		type;
	struct s_ast	*left;
	struct s_ast	*right;
	long int		exit_status;
	t_commands		*cmd;
	int				operator_type;
	t_token			*token;
	char			**heredoc_files;
	int				heredoc_count;
	t_env			*env_list;
}	t_ast;

typedef struct s_signal
{
	struct sigaction		sa_new;
	struct sigaction		sa_old;
	volatile sig_atomic_t	sigint;
}	t_signal;

/* Replace this in your minishell.h file */
typedef int		(*t_builtin_func)(t_ast *, t_ast *, int);

typedef struct s_builtin
{
	const char		*name;
	t_builtin_func	func;
}	t_builtin;

typedef struct s_assign_context
{
	bool			in_assignment;
	bool			after_equals;
	int				equal_pos;
	t_token			*assign_start;
}	t_assign_context;

typedef struct s_validation_context
{
	t_paren				*command;
	t_assign_context	*ctx;
	char				*input;
	t_token				*token;
}	t_validation_context;

typedef struct s_quote_state
{
	char		*result;
	int			i;
	int			j;
	int			in_quotes;
	char		quote_char;
}	t_quote_state;

extern pid_t	g_child_pid;
int	handle_file_error(char *filename);
int  create_heredoc_file(t_ast *data, t_redir_list *redir);
char		*redir_path(t_ast *n);
int	setup_fds(t_ast *data, t_ast *tree, int *fd_in, int *fd_out);
int	cd_too_many_args(t_ast *data);
int			open_infile(char *path);
int			open_outfile(char *path, int type);
void		create_intermediate_outfile(char *path, int type);
void		cleanup_heredoc_files(t_ast *data);
int			handle_line(int fd, char *line, char *delim);
int			run_heredoc_loop(int fd, char *delim);
int			fork_heredoc(int fd, char *delim);
int			setup_heredoc_filename(t_ast *data, t_ast *node, char *tmp);
void		update_last_exit_status(int status);
int			get_last_exit_status(void);
char		*join_path(char *dir_part, char *name);
char		**add_match(char **matches, int *count, char *path);
int			match_pattern(const char *pat, const char *str);
t_env		*new_node(const char *key, const char *val);
int			update_env_value(t_env *lst, const char *key, const char *val);
int			add_env_node(t_env **lst, t_env *new_node);
int			add_heredoc(t_ast *data, char *path);
void		free_heredoc_list(t_ast *data);
int			setup_output_fd(t_ast *data, t_ast *node);
int			create_heredoc_temp_file(t_ast *data, t_ast *node);
int			execute_redirections(t_ast *data, t_ast *node);
void		reset_heredoc_list(t_ast *data);
void		free_heredoc_list(t_ast *data);
int			split_key_value(const char *arg, char **key, char **val);
t_env		*find_env_node(t_env *lst, const char *key);
int			set_or_append_env(t_env **env_list,
				const char *key, const char *value);
int			is_valid_identifier(const char *s);
int			builtin_env(t_ast *data, t_ast *tree, int fd_out);
int			builtin_pwd(t_ast *data, t_ast *tree, int fd_out);
int			builtin_unset(t_ast *data, t_ast *tree, int fd_out);
int			builtin_export(t_ast *data, t_ast *tree, int fd_out);
void		create_new_shlvl(t_env **data, int shlvl);
void		setup_child_signals(void);
void		display_command_not_found(char *cmd);
int			builtin_cd(t_ast *data, t_ast *tree,int fd);
void		update_env_var(t_ast *data, const char *key, const char *value);
int			execute_home(t_ast *data, char *path, char *oldpwd);
void		print_error(char *filename, char *error_msg);
void		child_process_handler(t_ast *data,
				t_ast *tree, int fd_inp, int fd_out);
void		close_pipe(int fd[2]);
int			setup_pipe(int fd[2]);
void		execute_command(t_ast *data, t_ast *tree, int fd_in, int fd_out);
void		setup_child_redirections(int fd_in, int fd_out);
int			parent_process(pid_t pid, t_ast *data);
void		close_fds(int fd_in, int fd_out);
int			is_hidden_file(char *pattern, const char *filename);
void		split_pattern(const char *pattern,
				char **dir_part, char **file_part);
int			has_wildcard(const char *str);
void		free_matches_array(char **matches);
char		**finalize_args_array(char **args, int count);
void		add_arg_to_args(char ***new_args, int *new_count, char *arg);
void		process_all_arguments(char **args,
				char ***new_args, int *new_count);
void		cleanup_resources(int fd_in, int fd_out, char **expanded_args);
t_env		*get_env_node(t_env *env_list, const char *name);
char		**finalize_matches(char **matches, int count);
char		**process_dir_entries(DIR *dir, char *file_part,
				char *dir_part, int *count);
char		**expand_wildcard(char *pattern);
char		**handle_no_wildcard(char *pattern);
char		*ft_strcpy(char *dest, const char *src);
char		*ft_strcat(char *dest, const char *src);
int			handle_pwd_errors(char *old_pwd, int error_code);
int			update_directory(t_ast *data, char *path, char *old_pwd);
void		incr_shell_lvl(t_env *data);
int			validate_assignment_value(char *assignment);
void		print_export_error(char *arg);
void		print_unset_error(char *name);
char		*get_export_name(char *arg, char *eq);
void		update_existing_env(t_env *env, char *eq, char *name);
int			create_new_env(t_ast *d, char *n, char *eq);
int			handle_cd_home(t_ast *data, char **path, char *old_pwd);
void		handle_cd_error(char *path, char *old_pwd);
void		free_data(t_ast *data);
void		ft_strdel(char **as);
char		*find_executable_path(t_ast *data, char *cmd);
char		**expand_wildcards_in_args(char **args);
int			get_output_file(t_ast *tree);
char		**env(t_env **lst);
int			execute_or(t_ast *data, t_ast *tree);
int			execute_and(t_ast *data, t_ast *tree);
int			execute_pipe(t_ast *data, t_ast *tree);
int			handle_builtin(t_ast *data, t_ast *tree, int fd_out);
int			execute_word(t_ast *data, t_ast *tree);
char		*ft_strndup(const char *s, size_t n);
char		*ft_strjoin3(const char *s1, const char *s2, const char *s3);
int			execute_export(t_ast *data, t_ast *tree, int fd_out);
int			execute_unset(t_ast *data, t_ast *tree);
void		print_env_list(t_env *env_list, int fd_out);
int			execute_cd(t_ast *data, char *path);
void		set_env_var(t_ast *data, char *var_name, const char *var_value);
int			execute_pwd(t_ast *data, int fd_out);
int			execute_echo(char *args[], int fd_out);
int			execute_exit(t_ast *data, t_ast *tree);
t_env		*create_env_node(char *input);
t_token		*create_node(char *str, t_token_type type);
void		shift_arguments(char **args, int shift);
t_token		*tokenize(char *input);
int			handle_assignment(t_token **token, char *input, int *i);
void		append_node(t_token **token, t_token *current_token);
int			is_operator(char c);
int			return_parenthesis(t_token **token, char c);
int			handle_double_operator(t_token **head, char *input, int *i);
int			handle_single_operator(t_token **token, char c);
int			handle_word(t_token **token, char *input, int *i);
int			handle_quotes(t_token **token, char *input, int *i);
int			handle_whitespace(t_token **token, char *input, int *i);

// Environmental variables
t_env		*init_env_list(char **envp);
int			quotes(char	*input, int i, t_args *parse);
char		*safe_and_expand_var(char *value);
int			validate_parentheses(char *str);
char		*handle_env_part(t_args *parse, int *i, t_env *env_list);
void		process_env_var(t_args *parse, t_env *env_list, char *input);
char		*gen_env_value(t_env *env_list, char *key);
char		*parse_env(char *input, t_env *env_list, t_args *arg);
char		*get_env_value(t_env *env_list, char *name);
char		*join_arguments(t_args *arg);
char		*env_expansion(char *input, int *i, t_env *env_list, t_args *arg);
t_env		*deep_copy_env_list(t_env *env_list);
char		*strip_quotes_and_parens_tokens(t_token *tokens);
char		*remove_quotes_and_paren(char *str);

// Validation
bool		validation(t_token *tokens);
bool		validation_parenthesis(t_token *tokenize);

// Parenthesis
bool		check_parenthesis(t_validation_context *vctx, int i);
bool		is_in_quotes(t_quotes *quote);
bool		find_command_before_paren(char *input, int i, t_paren *command);
bool		check_operator_before_command(char *input, t_paren *command);
bool		check_command_paren_sequence(t_token *curr, t_token *prev,
				t_assign_context *ctx);
bool		check_command_before(char *input, int i, t_token *token);
void		process_quotes(char c, t_quotes *quote);
void		process_quotes_enhanced(char c, char prev_char, t_quotes *quote);
bool		is_in_assignment_value(t_assign_context *ctx, int pos);
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
bool		contains_assignment(const char *value, int *equal_pos);
bool		check_after_close_paren(char *input, int *i, t_token *token);
bool		it_is_log_or_pipe(char *input, int *i, t_token *token);
bool		it_is_logical_op(char *input, int *i, int *j, t_token *token);
bool		it_is_pipe(char *input, int *i, int *j, t_token *token);
bool		is_assignment_command(const char *value);
void		handle_assignment_token(t_assign_context *ctx, t_token *token);
void		reset_assign(t_assign_context *ctx);
bool		check_paren_syntax(t_token *curr, t_token *prev,
				t_assign_context *ctx);
void		update_assignment_context(t_assign_context *ctx, t_token *current);

// Operators
bool		ft_is_operator(char c);
bool		ft_is_logical_op(char current, char next);
bool		validate_logical_op(t_token *tokens);

// Redirections
bool		ft_is_redirection(char c);
bool		ft_is_redirection_op(char current, char next);
bool		validate_next_redirect(t_token **next);

//Commands
bool		ft_is_commands_position(char *input, int i);
bool		validate_commands(t_token *tokenize);
bool		is_valid_command_start(char c);
bool		is_valid_command_char(char c);
bool		is_command_or_arg_char(char c);
bool		is_valid_flag_char(char c);
bool		process_flag(char *str, int i,
				t_flag_state *flag_state);

// Wildcards
bool		ft_is_wildcard(char c);

// Pipes
bool		pipes(t_token *tokenize);
bool		is_operator_token(t_token *prev);
bool		has_whitespace_between(t_token *prev, t_token *curr);
bool		is_only_whitespaces(char *str);
bool		return_pipes_mssg(bool *expecting_cmd);
bool		check_redirection_before_pipe(t_token *curr, t_token *head);
bool		is_noclobber_operator(t_token *redir, t_token *pipe);
bool		check_is_noclobber_operator(t_token *prev, t_token *curr);

// Redirect
bool		validate_redirection(t_token *tokenize);
bool		check_after_redirection(t_token **curr);

// Stack operations
void		push(t_token *stack, char data);
void		initialize_stack(t_token *stack);
char		pop(t_token *stack);
bool		is_empty(t_token *stack);
char		peek(t_token *stack);

// Errors and Free functions
void		free_env_list(t_env *env_list);
t_token		*free_tokens(t_token *token);
void		report_error(t_errors_code code, char *token);
void		free_stack(t_token *token);

// Binary Tree
t_ast		*parse_tokens(t_token *tokens);
t_ast		*parse_command_line(t_token **curr);
t_ast_type	get_redir_type(t_token *tokens);
t_redir_list *create_redir_node(int type, char *filename);
t_ast		*parse_command(t_token **tokens);
t_ast		*parse_logic_sequence(t_token **tokens);
t_ast		*create_ast_node(t_ast_type type, t_token *token);
void		free_ast(t_ast *node);
t_ast		*parse_simple_commands(t_token **tokens);
t_commands	*create_command_struct(void);
char		**expand_command_args(char **temp_args, int temp_count);
t_ast		*create_command_node(t_token *start, int word_count);
void		skip_tree_whitespaces(t_token **tokens);
void		*skip_node(t_token **tokens, t_token *curr);
int			parse_redirection(t_token **tokens, t_ast *cmd_node);
t_ast		*parse_subshell(t_token **tokens);
t_ast		*parse_pipeline_node(t_ast *left, t_token **tokens);
int			handle_paren_token(t_expand_wild *exp, char ***temp_args);

// Utils functions
int			ft_strcmp(const char *s1, const char *s2);
int			count_parenthesis(t_token *tokens);
int			ft_isspace(int num);
void		clear_data(t_env **data, char **envp);
void		handle_signal(void);
void		free_2darray(char **array);
int			execute_tree(t_ast *data, t_ast *tree);

// Main functions
char		*generate_prompt(void);
void		handle_input(char *input, t_env *env_list, int argc, char **argv);

#endif
