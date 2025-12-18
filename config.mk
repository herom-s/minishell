NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = src
INC_DIR = inc
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I./$(INC_DIR) -I$(LIBFT_DIR)/inc

LEX_DIR = lexer
PARSER_DIR = parser
EVAL_DIR = eval
HASHTABLE_DIR = hashtable
SRC_FILES = main.c \
			print.c \
			$(LEX_DIR)/lexer.c \
			$(LEX_DIR)/lexer_helpers.c \
			$(LEX_DIR)/token.c \
			$(PARSER_DIR)/parser.c \
			$(EVAL_DIR)/eval.c \
			$(EVAL_DIR)/util.c \
			$(EVAL_DIR)/cmd/cmd.c \
			$(EVAL_DIR)/cmd/cmd_str.c \
			$(EVAL_DIR)/cmd/cmd_str_helpers.c \
			$(EVAL_DIR)/shell_env/shell_env.c \
			$(EVAL_DIR)/builtin/cd.c \
			$(EVAL_DIR)/builtin/echo.c \
			$(EVAL_DIR)/builtin/env.c \
			$(EVAL_DIR)/builtin/env_util.c \
			$(EVAL_DIR)/builtin/exit.c \
			$(EVAL_DIR)/builtin/export.c \
			$(EVAL_DIR)/builtin/export_args_util.c \
			$(EVAL_DIR)/builtin/pwd.c \
			$(EVAL_DIR)/builtin/unset.c \
			$(HASHTABLE_DIR)/hashtable.c \
			$(HASHTABLE_DIR)/hashtable_item.c \
			$(HASHTABLE_DIR)/hashtable_iterator.c \
			$(HASHTABLE_DIR)/hashtable_util.c

BONUS_DIR = bonus
BONUS_OBJ_DIR = obj_bonus
