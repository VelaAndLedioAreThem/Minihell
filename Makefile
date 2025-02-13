# Name
NAME = minishell

# Compiler Settings
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDE = -I include/ -I libft/include
LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)/libft.a

# Directories
SRC = src/
PARSE = $(SRC)parse/
EXEC = $(SRC)exec/

# Color Codes
RESET = \033[0m
RED = \033[0m;31m
GREEN = \033[0m;32m

# Source files for minishell
SRCS = $(SRC)main.c $(PARSE)tokenize.c

# Object files
OBJS = $(SRCS:.c=.o)

# Main target
all: $(LIBFT) $(NAME)

# Compiling libft
$(LIBFT):
		@echo "$(GREEN)Compiling libft...$(RESET)"
		@$(MAKE) -C $(LIBFT_DIR)
		@echo "$(GREEN)Libft compiled successfully$(RESET)"

# Compiling minishell
$(NAME): $(OBJS) $(LIBFT)
		@echo "$(GREEN)Compiling minishell...$(RESET)"
		$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -L$(LIBFT_DIR) -lft -o $(NAME)
		@echo "$(GREEN)Minishell compiled successfully$(RESET)"

# Compile individual object files
%.o: %.c
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

# Phony targets
.PHONY: all clean fclean re
