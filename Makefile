# Name
NAME = minishell

# Compiler Settings
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=leak,address
INCLUDE = -I include/ -I libft/include
LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)/libft.a

# Directories
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
QUOTES = $(VALIDATION)quotes/
WILDCARD = $(VALIDATION)wildcards/
PIPES = $(VALIDATION)pipes/

# Color Codes
RESET = \033[0m
RED = \033[0m;31m
GREEN = \033[0m;32m

# Source files for minishell
SRCS = $(SRC)main.c $(TOKENIZE)tokenize.c $(NODES)nodes.c \
	   $(TOKENIZE)tokenize_utils.c $(PARSE_UTILS)utils.c $(PARSE_UTILS)errors_free.c \
	   $(TOKENIZE)tokenize_utils_2.c $(ENV)convert_env.c $(ENV)env_parsing.c \
	   $(ENV)env_parsing_utils.c $(STACK)stack.c $(VALIDATION)validation.c \
	   $(PARENTHESIS)parenthesis_validation.c $(PARENTHESIS)parenthesis.c $(PARENTHESIS)parenthesis_utils.c \
	   $(COMMANDS)validate_commands.c $(COMMANDS)commands_utils.c $(OPERATORS)operators_utils.c \
	   $(OPERATORS)validation_logical_operators.c $(REDIRECTION)redirections_utils.c $(QUOTES)validation_quotes.c \
	   $(WILDCARD)wildcards_utils.c $(PARENTHESIS)parenthesis_utils_1.c $(PARENTHESIS)parenthesis_utils_2.c \
	   $(PARENTHESIS)parenthesis_content.c $(PARENTHESIS)close_paren.c $(PARENTHESIS)close_paren_utils.c \
	   $(PARENTHESIS)close_paren_utils_1.c $(PARENTHESIS)find_match_paren.c $(PIPES)validation_pipes.c\

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
	valgrind --leak-check=full ./minishell

# Phony targets
.PHONY: all clean fclean re
