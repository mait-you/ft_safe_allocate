# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/19 16:30:00 by mait-you          #+#    #+#              #
#    Updated: 2025/04/26 15:42:14 by mait-you         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Library name 
NAME				:= ft_safe_allocate.a
FENCING_LIB			:= ft_safe_allocate_fenced.a

# Compiler and flags
CC					:= cc
CFLAGS				:= -Wall -Wextra -Werror
FENCING_FLAGS		:= -DMEMORY_FENCING=true

# Directory structure
OBJS_DIR			:= obj
FENCING_DIR			:= obj/fencing

# Source files
SRCS				:= ft_safe_allocate.c ft_safe_allocate_action.c \
    ft_safe_allocate_utils.c ft_safe_allocate_cleanup.c memory_fencing.c

# Header files
HEADERS				:= ft_safe_allocate.h

# Object files
OBJS				:= $(SRCS:%.c=$(OBJS_DIR)/%.o)
FENCING_OBJS		:= $(SRCS:%.c=$(FENCING_DIR)/%.o)

# Colors for terminal output
RESET				:= \033[0m
GREEN				:= \033[32m
YELLOW				:= \033[33m
BLUE				:= \033[34m
RED					:= \033[31m

# Default target
all: $(NAME)

# Create library archive
$(NAME): $(OBJS)
	@echo "$(GREEN)Creating library archive...$(RESET)"
	@ar rcs $(NAME) $(OBJS)
	@echo "$(GREEN)Library $(NAME) compiled successfully$(RESET)"

# Compile source files to object files
$(OBJS_DIR)/%.o: %.c $(HEADERS) | $(OBJS_DIR)
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Memory fencing variant
fencing: $(FENCING_LIB)

$(FENCING_LIB): $(FENCING_OBJS)
	@echo "$(GREEN)Creating memory-fenced library archive...$(RESET)"
	@ar rcs $(FENCING_LIB) $(FENCING_OBJS)
	@echo "$(GREEN)Memory-fenced library $(FENCING_LIB) compiled successfully$(RESET)"

$(FENCING_DIR)/%.o: %.c $(HEADERS) | $(FENCING_DIR)
	@echo "$(BLUE)Compiling fenced $<...$(RESET)"
	@$(CC) $(CFLAGS) $(FENCING_FLAGS) -c $< -o $@

# Create directories
$(OBJS_DIR):
	@mkdir -p $@

$(FENCING_DIR):
	@mkdir -p $@

# Clean object files
clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJS_DIR)
	@echo "$(GREEN)Object files cleaned$(RESET)"

# Clean object files and library
fclean: clean
	@echo "$(RED)Removing libraries...$(RESET)"
	@rm -f $(NAME) $(FENCING_LIB)
	@echo "$(GREEN)Libraries cleaned$(RESET)"

# Rebuild everything
re: fclean all

# Install the library to the system
install: $(NAME)
	@echo "$(YELLOW)Installing $(NAME) to /usr/local/lib...$(RESET)"
	@sudo cp $(NAME) /usr/local/lib/
	@sudo cp $(HEADERS) /usr/local/include/
	@echo "$(GREEN)Installation complete$(RESET)"

# Uninstall the library from the system
uninstall:
	@echo "$(YELLOW)Uninstalling $(NAME) from /usr/local/lib...$(RESET)"
	@sudo rm -f /usr/local/lib/$(NAME)
	@sudo rm -f /usr/local/include/$(HEADERS)
	@echo "$(GREEN)Uninstallation complete$(RESET)"

# Install the library to the system
install_fenced: $(FENCING_LIB)
	@echo "$(YELLOW)Installing $(FENCING_LIB) to /usr/local/lib...$(RESET)"
	@sudo cp $(FENCING_LIB) /usr/local/lib/
	@sudo cp $(HEADERS) /usr/local/include/
	@echo "$(GREEN)Installation complete$(RESET)"

# Uninstall the library from the system
uninstall_fenced:
	@echo "$(YELLOW)Uninstalling $(FENCING_LIB) from /usr/local/lib...$(RESET)"
	@sudo rm -f /usr/local/lib/$(FENCING_LIB)
	@sudo rm -f /usr/local/include/$(HEADERS)
	@echo "$(GREEN)Uninstallation complete$(RESET)"

# Phony targets
.PHONY: clean install uninstall