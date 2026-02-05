NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I./inc -I$(LIBFT_DIR)/inc
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc
SRC_FILES = main.c
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Libft
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Bonus
BONUS_DIR = src_bonus
BONUS_OBJ_DIR = obj_bonus
BONUS_FILES = main_bonus.c
BONUS_SRC = $(addprefix $(BONUS_DIR)/, $(BONUS_FILES))
BONUS_OBJ = $(BONUS_SRC:$(BONUS_DIR)/%.c=$(BONUS_OBJ_DIR)/%.o)

# CMocka Testing
CMOCKA_DIR = cmocka
CMOCKA_BUILD = $(CMOCKA_DIR)/build
CMOCKA_LIB = $(CMOCKA_BUILD)/src/libcmocka.a
CMOCKA_INCLUDE = $(CMOCKA_DIR)/include
CMOCKA_VERSION = 1.1.7

TEST_DIR = tests
TEST_OBJ_DIR = obj_tests
TEST_FILES = test_main.c
TEST_SRC = $(addprefix $(TEST_DIR)/, $(TEST_FILES))
TEST_OBJ = $(TEST_SRC:$(TEST_DIR)/%.c=$(TEST_OBJ_DIR)/%.o)
TEST_BIN = run_tests

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

valgrind-run:
	@valgrind -q\
		--leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--track-fds=yes \
		--trace-children=yes \
		--trace-children-skip='*/bin/*,*/sbin/*,/usr/bin/*' \
		--suppressions=./valgrind.supp \
		./$(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BONUS_OBJ_DIR)
	@rm -rf $(TEST_OBJ_DIR)
	@make -C $(LIBFT_DIR) clean
	@echo "[OK] Object files removed"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(TEST_BIN)
	@rm -rf $(CMOCKA_DIR)
	@make -C $(LIBFT_DIR) fclean
	@echo "[OK] $(NAME) removed"

re: fclean all

# Check for development tools
check-tools:
	@echo "[INFO] Checking for development tools..."
	@command -v c_formatter_42 >/dev/null 2>&1 || { \
		echo "[WARNING] c_formatter_42 not found"; \
		echo "[INFO] Install with: pip3 install --user 42-formatter"; \
		echo ""; \
	}
	@command -v norminette >/dev/null 2>&1 || { \
		echo "[WARNING] norminette not found"; \
		echo "[INFO] Install with: pip3 install --user norminette"; \
		echo ""; \
	}
	@if command -v c_formatter_42 >/dev/null 2>&1 && command -v norminette >/dev/null 2>&1; then \
		echo "[OK] All development tools are installed"; \
	else \
		echo "[INFO] Some tools are missing. Install them for full functionality."; \
	fi

# Git hooks setup
setup: check-tools
	@bash scripts/setup-hooks.sh

.PHONY: all clean fclean re bonus test setup check-tools

