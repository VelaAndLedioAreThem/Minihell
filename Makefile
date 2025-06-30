# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: codespace <codespace@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/24 19:19:51 by ldurmish          #+#    #+#              #
#    Updated: 2025/06/22 13:49:59 by ldurmish         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name
NAME = minishell

# Compiler Settings
CC = cc
CFLAGS = -Wall -Wextra -Werror -O0 -g
INCLUDE = -I include/ -I libft/include
LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)/libft.a

# Directories
BUILTINS = $(EXEC)builtins/
EXEC_UTILS = $(EXEC)utils/
SIGNALS = $(EXEC)signals/
EXEC = $(SRC)execution/
EXECUTION= $(EXEC)main_exec/
EXEC_ENV = $(EXEC)env/
WILDCARDS = $(EXEC)wildcards/
SRC = src/
PARSE = $(SRC)parse/
ENV = $(PARSE)env/
STACK = $(PARSE)stack/
TOKENIZE = $(PARSE)tokenize/
PARSE_UTILS = $(PARSE)utils/
NODES = $(PARSE)nodes/
VALIDATION = $(PARSE)validation/
PARENTHESIS = $(VALIDATION)parenthesis/
COMMANDS = $(VALIDATION)commands/
OPERATORS = $(VALIDATION)operators/
REDIRECTION = $(VALIDATION)redirections/
WILDCARD = $(VALIDATION)wildcards/
PIPES = $(VALIDATION)pipes/
TREE = $(SRC)tree/
TREE = src/tree/

# Color Codes
RESET = \033[0m
RED = \033[31m
GREEN = \033[32m

# Source files for minishell
SRCS = $(SRC)main.c $(SRC)main_utils.c $(TREE)tree.c $(TOKENIZE)tokenize.c \
	   $(NODES)nodes.c $(TOKENIZE)tokenize_utils.c $(PARSE_UTILS)utils.c \
	   $(PARSE_UTILS)errors_free.c $(PARSE_UTILS)utils_1.c $(TOKENIZE)tokenize_utils_2.c $(TOKENIZE)tokenize_utils_1.c $(ENV)convert_env.c \
	   $(ENV)env_parsing.c $(ENV)env_parsing_utils.c $(STACK)stack.c \
	   $(VALIDATION)validation.c $(PARENTHESIS)parenthesis_validation.c $(PARENTHESIS)parenthesis.c \
	   $(PARENTHESIS)parenthesis_utils.c $(COMMANDS)validate_commands.c $(COMMANDS)commands_utils.c \
	   $(OPERATORS)operators_utils.c $(OPERATORS)validation_logical_operators.c $(REDIRECTION)redirections_utils.c \
	   $(WILDCARD)wildcards_utils.c $(PARENTHESIS)parenthesis_utils_1.c \
	   $(PARENTHESIS)parenthesis_utils_2.c $(PARENTHESIS)parenthesis_content.c $(PARENTHESIS)close_paren.c \
	   $(PARENTHESIS)close_paren_utils.c $(PARENTHESIS)close_paren_utils_1.c $(PARENTHESIS)find_match_paren.c \
	   $(PIPES)validation_pipes.c $(PIPES)pipes_utils.c $(REDIRECTION)validate_redirection.c \
	   $(PIPES)check_redirection_before_pipes.c $(SIGNALS)signals.c \
	   $(EXECUTION)exec_handling.c $(EXECUTION)execute_external.c $(EXECUTION)execute_external_utils.c \
	   $(EXECUTION)execute_pipe.c $(EXECUTION)execute_pipe_utils.c $(EXECUTION)exec_word/execute_word.c \
	   $(EXECUTION)exec_word/execute_word_utils.c $(EXECUTION)exec_word/execute_word_utils2.c $(EXECUTION)execute.c \
	   $(WILDCARDS)expand_wildcard.c $(WILDCARDS)expand_wildcard_utils.c $(WILDCARDS)expand_wildcard_utils2.c $(WILDCARDS)expand_wildcard_utils3.c \
	   $(BUILTINS)builtins_utils.c $(BUILTINS)builtins.c $(EXEC_ENV)utils/builtins_env_utils.c $(EXEC_ENV)utils/builtins_env_utils_2.c \
	   $(BUILTINS)utils/builtins_path_utils.c $(BUILTINS)utils/builtins_path_utils_2.c $(BUILTINS)utils/builtins_path_utils_3.c $(BUILTINS)utils/builtins_path_utils_1.c $(EXEC)redirections/heredoc_handling_utils.c \
	   $(EXEC)redirections/redirection_utils_2.c $(EXEC)redirections/redirection_utils_1.c $(EXEC)redirections/redirection_utils_3.c $(EXEC)redirections/redirection_utils.c $(EXEC)redirections/heredoc.c $(TREE)parse_commands.c \
	   $(EXEC_UTILS)free.c $(EXEC_UTILS)utils4.c $(EXEC_UTILS)exit_status.c $(TREE)parse_redir.c \
	   $(TREE)tree_utils.c $(TREE)tree_utils_1.c $(EXEC_ENV)env_utils.c \
	   $(EXEC_ENV)env_utils_2.c $(EXEC_ENV)env_export.c $(EXEC_ENV)env_unset.c $(EXEC_ENV)export_utils.c \
	   $(TREE)parse_wildcards.c $(ENV)env_utils.c $(ENV)env_utils_1.c $(TREE)parse_wildcards_utils.c\
	   $(TOKENIZE)tokenize_assignment.c $(PARENTHESIS)assignment_paren.c $(TREE)parse_parenthesis.c\
	   $(PARENTHESIS)parenthesis_utils_3.c $(TREE)tree_utils_2.c $(TREE)tree_utils_3.c $(ENV)env_utils_2.c\
	   $(TREE)parse_utils_1.c $(ENV)tilde_expansion.c $(ENV)process_env_var.c \

# Object files
OBJS = $(SRCS:.c=.o)

# Main target
all: $(LIBFT) $(NAME)

# Compiling Libft
$(LIBFT):
		@echo "$(GREEN)Compiling libft...$(RESET)"
		@$(MAKE) -C $(LIBFT_DIR)
		@echo "$(GREEN)Libft compiled successfully$(RESET)"

# Compiling minishell
$(NAME): $(OBJS) $(LIBFT)
		@echo "$(GREEN)Compiling minishell...$(RESET)"
		$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)
		@echo "$(GREEN)Minishell compiled successfully$(RESET)"

# Compile individual object files
$(SRC)%.o: $(SRC)%.c
		$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Clean object files
clean:
		@echo "$(RED)Cleaning object files..$(RESET)"
		@rm -f $(OBJS)
		@$(MAKE) -C $(LIBFT_DIR) clean
		@echo "$(GREEN)Object files cleaned successfully!$(RESET)"

# Removing all (including executables)
fclean: clean
		@echo "$(RED)Cleaning executables...$(RESET)"
		@rm -f $(NAME)
		@$(MAKE) -C $(LIBFT_DIR) fclean
		@echo "$(GREEN)Executables cleaned successfully$(RESET)"

re: fclean all

# Valgrind
valgrind: re
	@echo "Running Minishell with Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell



# Phony targets
.PHONY: all clean fclean re