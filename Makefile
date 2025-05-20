# Compiler and compiler flags
CC = cc
CFLAGS     = -Wall -Wextra -Werror -Iinclude -g

# Directories
OBJ_DIR 	= obj
BIN_DIR 	= bin

# Target executable name
TARGET = $(BIN_DIR)/minishell

# Source files (manually listed)
SRC_FILES = main.c 

# Define the full source and object file paths (without using SRC_DIR)
SRC = $(SRC_FILES)
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Default target
all: directories $(TARGET) banner

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# Compile the final binary
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
	
banner:
	@printf "\033[1;31m"
	@printf "\n\n"
	@printf " ███▄ ▄███▓ ██▓ ███▄    █  ██▓   ▓█████▄ ▓█████ ▄▄▄      ▓█████▄ \n"
	@printf " ▓██▒▀█▀ ██▒▓██▒ ██ ▀█   █ ▓██▒   ▒██▀ ██▌▓█   ▀▒████▄    ▒██▀ ██▌\n"
	@printf " ▓██    ▓██░▒██▒▓██  ▀█ ██▒▒██▒   ░██   █▌▒███  ▒██  ▀█▄  ░██   █▌\n"
	@printf " ▒██    ▒██ ░██░▓██▒  ▐▌██▒░██░   ░▓█▄   ▌▒▓█  ▄░██▄▄▄▄██ ░▓█▄   ▌\n"
	@printf " ▒██▒   ░██▒░██░▒██░   ▓██░░██░   ░▒████▓ ░▒████▒▓█   ▓██▒░▒████▓ \n"
	@printf " ░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒ ░▓      ▒▒▓  ▒ ░░ ▒░ ░▒▒   ▓▒█░ ▒▒▓  ▒ \n"
	@printf " ░  ░      ░ ▒ ░░ ░░   ░ ▒░ ▒ ░    ░ ▒  ▒  ░ ░  ░ ▒   ▒▒ ░ ░ ▒  ▒ \n"
	@printf " ░      ░    ▒ ░   ░   ░ ░  ▒ ░    ░ ░  ░    ░    ░   ▒    ░ ░  ░ \n"
	@printf "        ░    ░           ░  ░        ░       ░  ░     ░  ░   ░    \n"
	@printf "                                   ░                       ░     \n"
	@printf "\033[0m"



# Compile object files
$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	@rm -rf $(OBJ_DIR)


# Clean everything
fclean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR) $(TESTER_DIR)

# Rebuild everything
re: fclean all

# Run the program
run: all
	$(TARGET) $(ARGS)


.PHONY: all clean fclean re run help directories
