#ifndef FT_SAFE_ALLOCATE_H
# define FT_SAFE_ALLOCATE_H

# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>

#define PROMPT "\e[1;31mprogram \e[1;32m▸ \e[0m"

# define SUCCESS 0
# define ERROR 1

# define WARN_FREE_NULL_PTR "\033[33mWarning: \033[0mattempt to free a NULL pointer\n"
# define ERR_MALLOC_FAILED "\033[31mError: \033[0mmemory allocation failed\n"

typedef struct s_allocation_node
{
	void						*ptr;
	size_t						size;
	struct s_allocation_node	*next;
}	t_allocation_node;

typedef enum e_action
{
	ALLOCATE,
	FREE_ALL,
	FREE_ONE,
	GET_USAGE,
	ADD_TO_TRACK,
}	t_action;

void	*ft_safe_allocate(size_t count, size_t size, t_action action, void *ptr);
void	*allocate_ptr(t_allocation_node **head, size_t count, size_t size);
void	*free_all(t_allocation_node **head);
void	*free_specific(t_allocation_node **head, const void *ptr);
size_t	get_allocation_count(t_allocation_node **head);
char	*add_to_tracking(t_allocation_node **head, void *ptr, size_t size);
void	*error_cleanup(t_allocation_node **head);
void	*ft_memset_sa(void *b, int c, size_t len);
void	*ft_calloc_sa(size_t count, size_t size);
void	ft_putstr_fd_sa(char *s, int fd);

#endif