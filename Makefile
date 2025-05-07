# Compiler and compiler flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -Iinclude -Isrc -Wunused

# Directories
OBJ_DIR 	= obj
BIN_DIR 	= bin
TESTER_DIR 	= tester

# Target executable name
TARGET = $(BIN_DIR)/philo

# Source files (manually listed)
SRC_FILES = main.c dinner.c getters_setters.c init.c monitor.c parsing.c safe_funktions.c \
            sync_utils.c utils.c

# Define the full source and object file paths (without using SRC_DIR)
SRC = $(SRC_FILES)
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Default target
all: directories $(TARGET)
	@echo -e "\033[1;32mProgram ready\033[0m ✅"

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# Compile the final binary
$(TARGET): $(OBJ)
	@echo -e "\033[1;33mLinking the program... \033[0m🌀"
	$(CC) $(CFLAGS) -o $@ $^

# Compile object files
$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	@rm -rf $(OBJ_DIR)
	@echo -e "\033[1;32mObject directory deleted\033[0m"

# Clean everything
fclean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR) $(TESTER_DIR)
	@echo -e "\033[1;32mEverything is deleted\033[0m"

# Rebuild everything
re: fclean all

# Run the program
run: all
	$(TARGET) $(ARGS)

# Clone Tester repository
tester: all
	@git clone https://github.com/MichelleJiam/LazyPhilosophersTester.git tester
	@cp $(TARGET) tester/philo
	@echo -e "\033[1;32mTester cloned and moved to ./bin\033[0m 🧪"


# Display help
help:
	@echo -e "\033[1;32mMakefile for C Philo\033[0m"
	@echo -e "\033[1;34mUsage:\033[0m"
	@echo -e " \033[1;33mmake\033[0m               		- Build the program"
	@echo -e " \033[1;33mmake clean\033[0m         		- Remove object files"
	@echo -e " \033[1;33mmake fclean\033[0m        		- Remove all files"
	@echo -e " \033[1;33mmake run ARGS=\"...\"\033[0m 	- Build and run with arguments"
	@echo -e " \033[1;33mmake tester\033[0m           	- Download philo tester"
	@echo -e " \033[1;33mmake help\033[0m           	- Display this help information"

.PHONY: all clean fclean re run help directories
