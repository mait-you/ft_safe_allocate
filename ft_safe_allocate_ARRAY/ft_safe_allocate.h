/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:44:47 by mait-you          #+#    #+#             */
/*   Updated: 2025/04/20 18:09:29 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SAFE_ALLOCATE_H
# define FT_SAFE_ALLOCATE_H

# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdbool.h>

# ifndef MEMORY_FENCING
#  define MEMORY_FENCING false
# endif

# define SUCCESS 0
# define ERROR 1

# define WARN_NEAR_ALLOC_LIMIT "Warning: nearing allocation limit"
# define WARN_FREE_NULL_PTR "Warning: attempt to free a NULL pointer"
# define WARN_PTR_NOT_ALLOCATED "Warning: pointer was not allocated by \
ft_safe_allocate"
# define ERR_ALLOC_TRACK_LIMIT "Error: allocation tracking limit reached"
# define ERR_CORRUPTION_START "Error: memory corruption detected at START \
guard byte"
# define ERR_CORRUPTION_END "Error: memory corruption detected at END \
guard byte"

# define MAX_ALLOCATIONS 1024
# define GUARD_SIZE 8
# define GUARD_PATTERN 0xAB

typedef struct s_allocation
{
	void	*original_ptr;
	void	*user_ptr;
	size_t	size;
}	t_allocation;

typedef enum e_action
{
	ALLOCATE,
	FREE_ALL,
	FREE_ONE,
	GET_USAGE,
	REALLOC
}	t_action;

void	*allocate_ptr_memory_fencing(size_t size[2], t_allocation *ptr_array);
void	*free_all_memory_fencing(t_allocation *ptr_array);
void	*free_specific_memory_fencing(t_allocation *ptr_array, const void *ptr);

void	*ft_memset(void *b, int c, size_t len);
void	*ft_calloc(size_t count, size_t size);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	ft_putendl_fd(char *s, int fd);
int		add_to_tracking(\
	t_allocation *ptr_array, void *original_ptr, \
	void *user_ptr, size_t size[2]);

void	*realloc_ptr(size_t size[2], t_allocation *ptr_array, void *ptr);
void	*allocate_ptr(size_t size[2], t_allocation *ptr_array);
void	*free_all(t_allocation *ptr_array);
void	*free_specific(t_allocation *ptr_array, const void *ptr);
int		get_allocation_count(t_allocation *ptr_array);

void	*ft_safe_allocate(size_t size[2], t_action action, void *ptr);

#endif
