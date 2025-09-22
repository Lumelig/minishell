# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/10 17:18:56 by jpflegha          #+#    #+#              #
#    Updated: 2025/07/07 18:38:17 by jpflegha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# === CONFIGURATION ===
NAME		= libft.a
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -MMD
OBJ_DIR		= obj

SRC			= ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_memcpy.c \
			  ft_memset.c ft_strlen.c ft_tolower.c ft_toupper.c ft_atoi.c ft_strlcpy.c ft_memmove.c \
			  ft_strlcat.c ft_strrchr.c ft_strchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
			  ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
			  ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putendl_fd.c ft_putstr_fd.c ft_putnbr_fd.c \
			  ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c ft_atol.c \
			  ft_isspace.c get_next_line.c ft_printf.c ft_print_ptr.c ft_strcmp.c ft_printf_utils.c \
			  ft_strcpy.c ft_strjoin_char.c \

BONUS		= ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c

OBJ			= $(SRC:%.c=$(OBJ_DIR)/%.o)
BONUS_OBJ	= $(BONUS:%.c=$(OBJ_DIR)/%.o)
DEPS		= $(OBJ:.o=.d) $(BONUS_OBJ:.o=.d)

# === COLORS (OPTIONAL) ===
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
RESET		= \033[0m

# === TARGETS ===
all: $(NAME)

$(NAME): $(OBJ)
	@printf "$(YELLOW)[AR]$(RESET) Archiving $@\n"
	@ar rcs $@ $^

# Object compilation
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@printf "$(GREEN)[CC]$(RESET) Compiling $<\n"
	@$(CC) $(CFLAGS) -c $< -o $@

bonus: $(OBJ) $(BONUS_OBJ)
	@printf "$(YELLOW)[AR]$(RESET) Archiving bonus to $(NAME)\n"
	@ar rcs $(NAME) $(OBJ) $(BONUS_OBJ)

clean:
	@printf "$(YELLOW)Cleaning object files...$(RESET)\n"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@printf "$(YELLOW)Removing library...$(RESET)\n"
	@rm -f $(NAME)

re: fclean all

# === INCLUDE DEPENDENCIES ===
-include $(DEPS)

# === PHONY TARGETS ===
.PHONY: all bonus clean fclean re
