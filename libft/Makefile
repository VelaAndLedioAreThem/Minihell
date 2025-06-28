# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ledio <ledio@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/19 12:20:49 by ldurmish          #+#    #+#              #
#    Updated: 2025/06/28 13:12:09 by ldurmish         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ╔══════════════════════════════════════════════════════════════════════╗
# ║                               LIBFT                                  ║
# ╚══════════════════════════════════════════════════════════════════════╝

# ────────────────────────────────────────────────────────────────────────
# 1.  Basic settings
# ────────────────────────────────────────────────────────────────────────
NAME        := libft.a

CC          := cc
CFLAGS      := -Wall -Wextra -Werror -Iinclude

AR          := ar rcs
RM          := rm -f

# ────────────────────────────────────────────────────────────────────────
# 2.  Directories & source lists
# ────────────────────────────────────────────────────────────────────────
SRC_DIR     := src
GNL_DIR     := gnl
BONUS_DIR   := bonus

# Core functions
SRCS        := $(wildcard $(SRC_DIR)/*.c)

# get_next_line (non-bonus version – swap to *_bonus.c if you prefer)
SRCS        += $(GNL_DIR)/get_next_line.c \
               $(GNL_DIR)/get_next_line_utils.c

# Objects build alongside the sources (simplest layout)
OBJS        := $(SRCS:.c=.o)

# ────────────────────────────────────────────────────────────────────────
# 3.  Bonus (list & objects)
# ────────────────────────────────────────────────────────────────────────
BONUS_SRCS  := $(wildcard $(BONUS_DIR)/*.c)
BONUS_OBJS  := $(BONUS_SRCS:.c=.o)

# ────────────────────────────────────────────────────────────────────────
# 4.  Targets
# ────────────────────────────────────────────────────────────────────────
all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $@ $^

bonus: $(NAME) $(BONUS_OBJS)   # add bonus objects to the archive
	$(AR) $@ $(BONUS_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(BONUS_OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus

