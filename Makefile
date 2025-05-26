# Compiler and compiler flags
CC = cc
CFLAGS     = -Wall -Wextra -Werror -Iinclude -I$(LIBFT_DIR) #-fsanitize=address -g

# Directories
OBJ_DIR 	= obj
BIN_DIR 	= bin
LIBFT_DIR	= libft

# Target executable name
TARGET = $(BIN_DIR)/minishell

# Source files (manually listed)
SRC_FILES = main.c parsing.c quote.c

# Define the full source and object file paths (without using SRC_DIR)
SRC = $(SRC_FILES)
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Default target
all: directories $(LIBFT_DIR)/libft.a $(TARGET) banner

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@if [ ! -d "$(LIBFT_DIR)" ]; then git clone https://github.com/Lumelig/libft.git $(LIBFT_DIR); fi

# Build the libft library
$(LIBFT_DIR)/libft.a:
	@$(MAKE) -C $(LIBFT_DIR)

# Compile the final binary
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -L$(LIBFT_DIR) -lreadline -o $@ $^ $(LIBFT_DIR)/libft.a
	
banner:
	
	@printf "\033[1;31m"
	@printf "\n\n"
	@printf " ███▄ ▄███▒ ██▒ ███▄   ██  ██▒    ██░ ██ ▓█████  ██▒     ██▓\n"
	@printf "▒██▒▀█▀ ██▒▒██▒ ██ ▀█  ██▒ ██▒   ▓██░ ██▒▓█   ▀ ▒██▒    ▒██▒\n"
	@printf "▒██    ▓██░▒██▒▓██▒ ▀█ ██▒▒██▒   ▒██▀▀██░▒███   ▒██░    ▒██░\n"
	@printf "▒██    ▒██ ░██░▓██▒  ▐▌██▒░██░   ░▓█ ░██ ▒▓█  ▄ ▒██░    ▒██░\n"
	@printf "▒██▒   ░██▒░██░▒██░   ▓██░░██░   ░▓█▒░██▓░▒████▒░██████▒░██████▒\n"
	@printf "░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒ ░▓      ▒ ░░▒░▒░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░\n"
	@printf "░  ░      ░ ▒ ░░ ░░   ░ ▒░ ▒ ░    ▒ ░▒░ ░ ░ ░  ░░ ░ ▒  ░░ ░ ▒  ░\n"
	@printf "░      ░    ▒ ░   ░   ░ ░  ▒ ░    ░  ░░ ░   ░     ░ ░     ░ ░\n"
	@printf "       ░    ░           ░  ░      ░  ░  ░   ░  ░    ░  ░    ░  ░\n"
	@printf "\033[0m"



# Compile object files
$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	@$(MAKE) -C $(LIBFT_DIR)

# Clean object files
clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(OBJ_DIR/obj)


# Clean everything
fclean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR) $(TESTER_DIR) $(LIBFT_DIR)

# Rebuild everything
re: clean all


.PHONY: all clean fclean re run help directories
