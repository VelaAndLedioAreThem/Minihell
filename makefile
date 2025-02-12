CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline

# Include all source files, including those in the builtins folder
SRC = main.c envs.c signals.c \
      builtins/builtins.c builtins/handle_builtins.c builtins/Redirections.c

OBJ = $(SRC:.c=.o)
NAME = minishell

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

# Rule for compiling .c files into .o files
%.o: %.c shell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
