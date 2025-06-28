# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: codespace <codespace@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/24 19:19:51 by ldurmish          #+#    #+#              #
#    Updated: 2025/06/28 13:20:01 by ldurmish         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                 Minihell                                     #
# **************************************************************************** #

# **************************************************************************** #
#                                   Minishell                                  #
# **************************************************************************** #

NAME        := minishell

# -----------------------------------------------------------------------------#
#  Directories                                                                 #
# -----------------------------------------------------------------------------#
SRC_DIR     := src
OBJ_DIR     := obj
INC_DIR     := include

LIBFT_DIR   := libft
LIBFT_A     := $(LIBFT_DIR)/libft.a

# -----------------------------------------------------------------------------#
#  Compiler & flags                                                            #
# -----------------------------------------------------------------------------#
CC          := cc
CFLAGS      := -Wall -Wextra -Werror
ifdef DEBUG                 # enable via `make DEBUG=1`
CFLAGS      += -g -fsanitize=address -fno-omit-frame-pointer
endif

INCLUDES    := -I$(INC_DIR) -I$(LIBFT_DIR)/include -I$(LIBFT_DIR)/gnl
LDLIBS      := $(LIBFT_A) -lreadline -ltermcap    # termcap needed on Linux

# -----------------------------------------------------------------------------#
#  Sources & objects                                                           #
# -----------------------------------------------------------------------------#
SRCS        := $(shell find $(SRC_DIR) -name "*.c")
OBJS        := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# -----------------------------------------------------------------------------#
#  Rules                                                                       #
# -----------------------------------------------------------------------------#
all: $(NAME)

# Build libft first (with get_next_line already in its SRCS)
$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

# Compile every .c under src/ -> obj/ (mirror dir tree)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Link everything into the final executable
$(NAME): $(LIBFT_A) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o $(NAME)

# -----------------------------------------------------------------------------#
#  House-keeping targets                                                       #
# -----------------------------------------------------------------------------#
clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

