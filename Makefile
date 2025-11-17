NAME = minishell
CC = gcc
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

# CMocka download and build
$(CMOCKA_LIB):
	@if [ -f "$(CMOCKA_LIB)" ]; then \
		echo "[OK] CMocka already installed"; \
	else \
		echo "[INFO] CMocka not found, installing..."; \
		if [ ! -d "$(CMOCKA_DIR)" ]; then \
			echo "[INFO] Downloading CMocka..."; \
			curl -L https://cmocka.org/files/1.1/cmocka-$(CMOCKA_VERSION).tar.xz -o cmocka.tar.xz && \
			tar -xf cmocka.tar.xz && \
			mv cmocka-$(CMOCKA_VERSION) $(CMOCKA_DIR) && \
			rm cmocka.tar.xz; \
		fi; \
		echo "[INFO] Building CMocka..."; \
		mkdir -p $(CMOCKA_BUILD) && \
		cd $(CMOCKA_BUILD) && \
		cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=OFF -DWITH_STATIC_LIB=ON .. && \
		cmake --build . --config Debug && \
		echo "[OK] CMocka built successfully"; \
	fi

# Test compilation
$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c $(CMOCKA_LIB)
	@mkdir -p $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) -I$(CMOCKA_INCLUDE) $(INCLUDES) -c $< -o $@

# Test binary
$(TEST_BIN): $(CMOCKA_LIB) $(TEST_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(TEST_OBJ) -L$(CMOCKA_BUILD)/src -lcmocka -L$(LIBFT_DIR) -lft -o $(TEST_BIN)
	@echo "[OK] Tests compiled successfully"

# Run tests
test: $(TEST_BIN)
	@echo "[INFO] Running tests..."
	@./$(TEST_BIN)

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
