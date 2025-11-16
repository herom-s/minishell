
NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I./inc -I$(LIBFT_DIR)

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

SRC_FILES = main.c
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# Libft
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
BONUS_DIR = src_bonus
BONUS_OBJ_DIR = obj_bonus
BONUS_FILES = main_bonus.c
BONUS_SRC = $(addprefix $(BONUS_DIR)/, $(BONUS_FILES))
BONUS_OBJ = $(BONUS_SRC:$(BONUS_DIR)/%.c=$(BONUS_OBJ_DIR)/%.o)

all: $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)
$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)
	@echo "[OK] $(NAME) compiled successfully"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

bonus: $(LIBFT) $(BONUS_OBJ)
	$(CC) $(CFLAGS) $(BONUS_OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)
	@echo "[OK] $(NAME) bonus compiled successfully"

$(BONUS_OBJ_DIR)/%.o: $(BONUS_DIR)/%.c
	@mkdir -p $(BONUS_OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BONUS_OBJ_DIR)
	@make -C $(LIBFT_DIR) clean
	@echo "[OK] Object files removed"

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@echo "[OK] $(NAME) removed"

re: fclean all

.PHONY: all clean fclean re bonus
