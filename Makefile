# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/07 19:22:52 by jpflegha          #+#    #+#              #
#    Updated: 2025/09/22 16:20:19 by mring            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name
NAME		= minishell

# Compiler and flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
DEBUG_FLAGS	= -fsanitize=address -g3
INCLUDES	= -Iinclude -I$(LIBFT_DIR)

# Colors for output
RED			= \033[0;31m
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
BLUE		= \033[0;34m
MAGENTA		= \033[0;35m
CYAN		= \033[0;36m
WHITE		= \033[0;37m
RESET		= \033[0m
BOLD		= \033[1m

# Directories
SRC_DIR		= src
OBJ_DIR		= obj
LIBFT_DIR	= libft

# Target executable
TARGET		= $(NAME)

# Source files
SRC_FILES = main.c \
			cleanup.c \
			signals.c \
			exit_code.c \
            parsing/tokenizer.c \
			parsing/tokenizer_utils.c \
			parsing/tokenizer_escape.c \
			parsing/tokenizer_word.c \
            parsing/quote.c \
            parsing/expand.c \
			parsing/expand_utils.c \
			parsing/expand_utils_2.c \
            parsing/env_init.c \
			parsing/env_utils.c \
			parsing/env_utils2.c \
			parsing/env_validate.c \
			parsing.c \
			parsing/token_to_cmd.c \
			parsing/cmd_utils.c \
			parsing/get_input.c \
			execution/builtin/builtin.c \
			execution/builtin/cd.c \
			execution/builtin/echo.c \
			execution/builtin/env.c \
			execution/builtin/exit.c \
			execution/builtin/export.c \
			execution/builtin/pwd.c \
			execution/builtin/unset.c \
			execution/executor.c \
			execution/fork_pipeline.c \
			execution/redirections.c \
			execution/heredoc.c \
			execution/cmd_exec.c \
			execution/cmd_exec_utils.c

# Add the src directory prefix to each file
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

# Object files
OBJ = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC:.c=.o))

# Header files for dependency tracking
HEADERS		= include/minishell.h include/parsing.h include/execution.h

# Libraries
LIBFT		= $(LIBFT_DIR)/libft.a
LIBS		= -lreadline


# Default target
all: $(LIBFT) $(TARGET)
	@printf "$(BOLD)$(RED)"
	@printf "\n"
	@printf " ███▄ ▄███▒ ██▒ ███▄   ██  ██▒    ██░ ██ ▓█████  ██▒     ██▓\n"
	@printf "▒██▒▀█▀ ██▒▒██▒ ██ ▀█  ██▒ ██▒   ▓██░ ██▒▓█   ▀ ▒██▒    ▒██▒\n"
	@printf "▒██    ▓██░▒██▒▓██▒ ▀█ ██▒▒██▒   ▒██▀▀██░▒███   ▒██░    ▒██░\n"
	@printf "▒██    ▒██ ░██░▓██▒  ▐▌██▒░██░   ░▓█ ░██ ▒▓█  ▄ ▒██░    ▒██░\n"
	@printf "▒██▒   ░██▒░██░▒██░   ▓██░░██░   ░▓█▒░██▓░▒████▒░██████▒░██████▒\n"
	@printf "░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒ ░▓      ▒ ░░▒░▒░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░\n"
	@printf "░  ░      ░ ▒ ░░ ░░   ░ ▒░ ▒ ░    ▒ ░▒░ ░ ░ ░  ░░ ░ ▒  ░░ ░ ▒  ░\n"
	@printf "░      ░    ▒ ░   ░   ░ ░  ▒ ░    ░  ░░ ░   ░     ░ ░     ░ ░\n"
	@printf "       ░    ░           ░  ░      ░  ░  ░   ░  ░    ░  ░    ░  ░\n"
	@printf "$(RESET)\n"
	@printf "$(GREEN)$(BOLD)Ready to execute: $(CYAN)./$(TARGET)$(RESET)\n\n"

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: fclean all

# noflag debug build
noflags: CFLAGS =
noflags: fclean all

# Build libft
$(LIBFT):
	@rm -rf libft/.git
	@printf "$(CYAN)Building libft...$(RESET)\n"
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory
	@printf "$(GREEN)Libft built successfully!$(RESET)\n"

# Compile object files with progress tracking
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@printf "$(BLUE)Compiling $(YELLOW)$<$(RESET)... "
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@printf "$(GREEN)✓$(RESET)\n"

# Link final executable
$(TARGET): $(OBJ) $(LIBFT)
	@printf "$(MAGENTA)Linking $(NAME)...$(RESET)\n"
	@$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBFT) $(LIBS)
	@printf "$(GREEN)$(BOLD)✓ $(NAME) built successfully!$(RESET)\n"

# Run the program
run: $(TARGET)
	@printf "$(CYAN)Running $(NAME)...$(RESET)\n"
	@./$(TARGET)

# install the minishell tester
tester:
	@printf "$(CYAN)Installing minishell tester...$(RESET)\n"
	@curl -fsSL https://raw.githubusercontent.com/zstenger93/42_minishell_tester/master/install.sh -o installer.sh
	@if [ -f installer.sh ]; then \
		printf "$(CYAN)Setting permissions and running installer...$(RESET)\n"; \
		chmod +x installer.sh; \
		echo "exit" | ./installer.sh; \
		printf "$(CYAN)Sourcing ~/.zshrc...$(RESET)\n"; \
		zsh -c "source ~/.zshrc"; \
	fi
	@printf "$(GREEN)Tester installation complete!$(RESET)\n"

# Run with valgrind
valgrind: $(TARGET)
	@printf "$(CYAN)Running $(NAME) with valgrind...$(RESET)\n"
	@valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=sub.sub ./$(TARGET)

# Clean object files
clean:
	@printf "$(YELLOW)Cleaning object files...$(RESET)\n"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory
	@printf "$(GREEN)Object files cleaned!$(RESET)\n"

# Clean everything
fclean:
	@printf "$(YELLOW)Cleaning everything...$(RESET)\n"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory
	@printf "$(GREEN)Everything cleaned!$(RESET)\n"

# Rebuild everything
re: fclean all

# Phony targets
.PHONY: all debug clean fclean re run valgrind noflags