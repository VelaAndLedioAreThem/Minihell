/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:04:11 by ldurmish          #+#    #+#             */
/*   Updated: 2025/02/19 22:12:29 by ldurmish         ###   ########.fr       */
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
	TOKEN_WILDCARD
}	t_token_type;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				expandable;
	struct s_token	*next;
}	t_token;

typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPELINE,
	AST_REDIR,
	AST_AND_OR,
	AST_SUBSHELL
}	t_ast_type;

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

// Validation
bool		validation(t_token *tokens);

// Free functions
void		free_env_list(t_env *env_list);
int			ft_isspace(int num);
t_token		*free_tokens(t_token *token);
char		*env_expansion(char *input, int *i, t_env *env_list, t_args *arg);

// Utils functions
int			ft_strcmp(char *s1, char *s2);

#endif
