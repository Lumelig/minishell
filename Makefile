# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: your_username <your_email@student.42.fr>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/01 12:00:00 by username          #+#    #+#              #
#    Updated: 2025/01/01 12:00:00 by username         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name
NAME		= minishell

# Compiler and flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -Iinclude -I$(LIBFT_DIR)
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
BIN_DIR		= bin
LIBFT_DIR	= libft
INCLUDE_DIR	= include

# Target executable
TARGET		= $(BIN_DIR)/$(NAME)

# Source files (add new .c files here)
SRC_FILES	= main.c \
			  parsing.c \
			  quote.c \
			  expand.c

# Object files
SRC			= $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ			= $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Header files for dependency tracking
HEADERS		= $(wildcard $(INCLUDE_DIR)/*.h)

# Libraries
LIBFT		= $(LIBFT_DIR)/libft.a
LIBS		= -lreadline -lhistory

# Progress tracking
TOTAL_FILES	= $(words $(SRC_FILES))
CURRENT		= 0

# Default target
all: directories $(LIBFT) $(TARGET) banner

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) $(INCLUDE_DIR)
	@if [ ! -d "$(SRC_DIR)" ]; then mkdir -p $(SRC_DIR); fi
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		printf "$(YELLOW)Cloning libft repository...$(RESET)\n"; \
		git clone https://github.com/Lumelig/libft.git $(LIBFT_DIR) 2>/dev/null || \
		(printf "$(RED)Failed to clone libft repository$(RESET)\n" && exit 1); \
	fi

# Build libft
$(LIBFT): | directories
	@printf "$(CYAN)Building libft...$(RESET)\n"
	@$(MAKE) -C $(LIBFT_DIR) re --no-print-directory
	@printf "$(GREEN)Libft built successfully!$(RESET)\n"

# Compile object files with progress tracking
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | directories
	@$(eval CURRENT=$(shell echo $$(($(CURRENT)+1))))
	@printf "$(BLUE)[$(CURRENT)/$(TOTAL_FILES)]$(RESET) Compiling $(YELLOW)$<$(RESET)... "
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@printf "$(GREEN)✓$(RESET)\n"

# Fallback for files in root directory (temporary compatibility)
$(OBJ_DIR)/%.o: %.c $(HEADERS) | directories
	@$(eval CURRENT=$(shell echo $$(($(CURRENT)+1))))
	@printf "$(BLUE)[$(CURRENT)/$(TOTAL_FILES)]$(RESET) Compiling $(YELLOW)$<$(RESET)... "
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@printf "$(GREEN)✓$(RESET)\n"

# Link final executable
$(TARGET): $(OBJ) $(LIBFT)
	@printf "$(MAGENTA)Linking $(NAME)...$(RESET)\n"
	@$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBFT) $(LIBS)
	@printf "$(GREEN)$(BOLD)✓ $(NAME) built successfully!$(RESET)\n"

# Beautiful banner
banner:
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

# Run the program
run: $(TARGET)
	@printf "$(CYAN)Running $(NAME)...$(RESET)\n"
	@./$(TARGET)

# Run with valgrind
valgrind: $(TARGET)
	@printf "$(CYAN)Running $(NAME) with valgrind...$(RESET)\n"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)


# Clean object files
clean:
	@printf "$(YELLOW)Cleaning object files...$(RESET)\n"
	@rm -rf $(OBJ_DIR)
	@if [ -d "$(LIBFT_DIR)" ]; then $(MAKE) -C $(LIBFT_DIR) clean --no-print-directory; fi
	@printf "$(GREEN)Object files cleaned!$(RESET)\n"

# Clean everything
fclean: clean
	@printf "$(YELLOW)Cleaning executable and directories...$(RESET)\n"
	@rm -rf $(BIN_DIR) $(LIBFT_DIR)
	@if [ -d "$(LIBFT_DIR)" ]; then $(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory; fi
	@printf "$(GREEN)Everything cleaned!$(RESET)\n"

# Rebuild everything
re: fclean all

# Phony targets
.PHONY: all debug clean fclean re run valgrind banner directories

# Silence make
.SILENT: