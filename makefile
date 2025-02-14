NAME = minishell

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes

SRC = $(wildcard $(SRC_DIR)/*.c) \
      $(wildcard $(SRC_DIR)/builtins/*.c) \
      $(wildcard $(SRC_DIR)/parsing/*.c) \
      $(wildcard $(SRC_DIR)/execution/*.c) \
      $(wildcard $(SRC_DIR)/utils/*.c)

OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lreadline

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR) $(OBJ_DIR)/builtins $(OBJ_DIR)/parsing $(OBJ_DIR)/execution $(OBJ_DIR)/utils
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re