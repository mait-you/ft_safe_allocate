# Simple Makefile for ft_safe_allocate library

NAME = libft_safe_allocate.a

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c99
AR = ar rcs

# Source files
SRCS = ft_safe_allocate/ft_safe_allocate.c \
       ft_safe_allocate/ft_safe_allocate_action.c \
       ft_safe_allocate/ft_safe_allocate_utils.c

# Object files
OBJS = $(SRCS:.c=.o)


HEADER = ft_safe_allocate.h

GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Creating library $(NAME)...$(RESET)"
	@$(AR) $(NAME) $(OBJS)
	@echo "$(GREEN)Library $(NAME) created successfully!$(RESET)"

%.o: %.c $(HEADER)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@
clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -f $(OBJS)
fclean: clean
	@echo "$(RED)Cleaning library...$(RESET)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re test