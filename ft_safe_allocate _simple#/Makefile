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
SRCS				:= ft_safe_allocate/ft_safe_allocate.c \
						ft_safe_allocate/ft_safe_allocate_action.c \
						ft_safe_allocate/ft_safe_allocate_utils.c \
						ft_safe_allocate/ft_safe_allocate_cleanup.c \
						ft_safe_allocate/memory_fencing.c

# Header files
HEADERS				:= include/ft_safe_allocate.h

# Object files
OBJS				:= $(SRCS:%.c=$(OBJS_DIR)/%.o)
FENCING_OBJS		:= $(SRCS:%.c=$(FENCING_DIR)/%.o)

# Colors for terminal output
RESET				:= \033[0m
GREEN				:= \033[1;32m
YELLOW				:= \033[4;33m
BLUE				:= \033[1;34m
RED					:= \033[1;31m
GRAYL				:= \033[3;90m

# Default target
all: $(NAME)

# Create library archive
$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(GREEN)Library $(YELLOW)$(NAME)$(RESET) $(GREEN)created successfully!$(RESET)"

# Compile source files to object files
$(OBJS_DIR)/%.o: %.c $(HEADERS) | $(OBJS_DIR)
	@mkdir -p $(dir $@)
	@echo "$(BLUE)Compiling: $(RESET)$(GRAYL)$<$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Memory fencing variant
fencing: $(FENCING_LIB)

$(FENCING_LIB): $(FENCING_OBJS)
	@ar rcs $(FENCING_LIB) $(FENCING_OBJS)
	@echo "$(GREEN)Library $(YELLOW)$(FENCING_LIB)$(RESET) $(GREEN)created successfully!$(RESET)"

$(FENCING_DIR)/%.o: %.c $(HEADERS) | $(FENCING_DIR)
	@mkdir $(dir $@)
	@echo "$(BLUE)Compiling: $(RESET)$(GRAYL)$<$(RESET)"
	@$(CC) $(CFLAGS) $(FENCING_FLAGS) -c $< -o $@

# Create directories
$(OBJS_DIR):
	@mkdir -p $@

$(FENCING_DIR):
	@mkdir -p $@

# Clean object files
clean:
	@rm -rf $(OBJS_DIR)
	@echo "$(RED)>> Object files cleaned$(RESET)"

# Clean object files and library
fclean:
	@rm -rf $(OBJS_DIR)
	@rm -f $(NAME) $(FENCING_LIB)
	@echo "$(RED)>> Libraries cleaned$(RESET)"

# Rebuild everything
re: fclean all

# Install the library to the system
install: $(NAME)
	@echo "$(YELLOW)>> Installing $(NAME) to /usr/local/lib...$(RESET)"
	@sudo cp $(NAME) /usr/local/lib/
	@sudo cp $(HEADERS) /usr/local/include/
	@echo "$(GREEN)>> Installation complete$(RESET)"

# Uninstall the library from the system
uninstall:
	@echo "$(YELLOW)>> Uninstalling $(NAME) from /usr/local/lib...$(RESET)"
	@sudo rm -f /usr/local/lib/$(NAME)
	@sudo rm -f /usr/local/include/$(HEADERS)
	@echo "$(GREEN)>> Uninstallation complete$(RESET)"

# Install the library to the system
install_fenced: $(FENCING_LIB)
	@echo "$(YELLOW)>> Installing $(FENCING_LIB) to /usr/local/lib...$(RESET)"
	@sudo cp $(FENCING_LIB) /usr/local/lib/
	@sudo cp $(HEADERS) /usr/local/include/
	@echo "$(GREEN)>> Installation complete$(RESET)"

# Uninstall the library from the system
uninstall_fenced:
	@echo "$(YELLOW)>> Uninstalling $(FENCING_LIB) from /usr/local/lib...$(RESET)"
	@sudo rm -f /usr/local/lib/$(FENCING_LIB)
	@sudo rm -f /usr/local/include/$(HEADERS)
	@echo "$(GREEN)>> Uninstallation complete$(RESET)"

# Phony targets
.PHONY: all clean fclean re fencing install uninstall install_fenced uninstall_fenced