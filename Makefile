# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/19 16:30:00 by mait-you          #+#    #+#              #
#    Updated: 2025/04/26 09:36:51 by mait-you         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Library name 
NAME        := ft_safe_allocate.a

# Compiler and flags
CC          := cc
CFLAGS      := -Wall -Wextra -Werror -fsanitize=address -g

# Source files
SRCS        := ft_safe_allocate.c ft_safe_allocate_action.c \
            ft_safe_allocate_utils.c ft_safe_allocate_cleanup.c memory_fencing.c

# Object files
OBJS_DIR    := obj
OBJS        := $(SRCS:%.c=$(OBJS_DIR)/%.o)

# Colors for terminal output
RESET       := \033[0m
GREEN       := \033[32m
YELLOW      := \033[33m
BLUE        := \033[34m
RED         := \033[31m

# Default target
all: $(NAME)

# Create library archive
$(NAME): $(OBJS)
	@echo "$(GREEN)Creating library archive...$(RESET)"
	@ar rcs $(NAME) $(OBJS)
	@echo "$(GREEN)Library $(NAME) compiled successfully$(RESET)"

# Compile source files to object files
$(OBJS_DIR)/%.o: %.c ft_safe_allocate.h | $(OBJS_DIR)
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create object directories
$(OBJS_DIR) $(DEBUG_DIR) $(TEST_DIR):
	@mkdir -p $@

# Memory fencing build
fencing: CFLAGS += -DMEMORY_FENCING=true
fencing: fclean $(OBJS)
	@echo "$(GREEN)Creating memory-fenced library archive...$(RESET)"
	@ar rcs $(NAME) $(OBJS)
	@echo "$(GREEN)Memory-fenced library $(NAME) compiled successfully$(RESET)"

# Clean object files
clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJS_DIR)
	@echo "$(GREEN)Object files cleaned$(RESET)"

# Clean object files and library
fclean: clean
	@rm -f $(NAME)
	@echo "$(GREEN)Library cleaned$(RESET)"

# Rebuild everything
re: fclean all

# Install the library to the system
install: $(NAME)
	@echo "$(YELLOW)Installing $(NAME) to /usr/local/lib...$(RESET)"
	@cp $(NAME) /usr/local/lib/
	@cp ft_safe_allocate.h /usr/local/include/
	@echo "$(GREEN)Installation complete$(RESET)"

# Phony targets
.PHONY: all clean fclean re fencing info install
