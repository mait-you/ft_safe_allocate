/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:44:47 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/26 15:52:53 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SAFE_ALLOCATE_H
# define FT_SAFE_ALLOCATE_H

/* Standard library includes */
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>

/* ************************************************************************** */
/* 							Configuration Parameters                          */
/* ************************************************************************** */

/**
 * @brief Size of the hash table for tracking allocations
 * Larger values reduce collision probability but increase memory usage
 */
# define HASH_TABLE_SIZE 2048

/**
 * @brief Terminal prompt definition that displays "program ▸" with color formatting
 *        - "program" appears in red
 *        - "▸" appears in green
 *        - Followed by a space in default terminal color
 */
#define PROMPT "\e[1;31mprogram \e[1;32m▸ \e[0m"

/* ************************************************************************** */
/* 							Status Codes and Messages                         */
/* ************************************************************************** */

/**
 * @brief Operation status codes
 */
# define SUCCESS 0
# define ERROR 1

/**
 * @brief Warning messages
 */
# define WARN_NEAR_ALLOC_LIMIT "\033[33mWarning: \033[0mnearing allocation \
limit\n"
# define WARN_BOTH_PTR "\033[33mWarning: \033[0mBoth ptr and double_ptr \
provided. Only ptr will be freed.\n"
# define WARN_FREE_NULL_PTR "\033[33mWarning: \033[0mattempt to free a NULL \
pointer\n"
# define WARN_PTR_NOT_ALLOCATED_1 "\033[33mWarning: \033[0m [0x "
# define WARN_PTR_NOT_ALLOCATED_2 "] was not allocated by ft_safe_allocate \
so it cannot be freed using it\n"

/**
 * @brief Error messages
 */
# define ERR_ALLOC_TRACK_LIMIT "\033[31mError: \033[0mallocation tracking \
limit reached\n"
# define ERR_MALLOC_FAILED "\033[31mError: \033[0mmemory allocation failed\n"

/* ************************************************************************** */
/* 							Data Structures                                   */
/* ************************************************************************** */

/**
 * @brief Forward declarations
 */
typedef struct s_allocation		t_allocation;

/**
 * @brief Structure to track memory allocations
 * 
 * @param user_ptr		Pointer provided to user
 * @param size			Size of allocated memory
 */
struct s_allocation
{
	void	*user_ptr;
	size_t	size;
};

/**
 * @brief Action enum for ft_safe_allocate function
 */
typedef enum e_action
{
	ALLOCATE,			/* Allocate new memory */
	FREE_ALL,			/* Free all tracked allocations */
	FREE_ONE,			/* Free a specific allocation */
	GET_USAGE,			/* Get count of current allocations */
	ADD_TO_TRACK,		/* Add externally allocated memory to tracking */
}	t_action;

/* ************************************************************************** */
/* 							Function Prototypes                               */
/* ************************************************************************** */


void	*ft_safe_allocate(\
	size_t count, size_t size, t_action action, void *ptr);
size_t	hash_ptr(const void *ptr);
void	*allocate_ptr(size_t count, size_t size, t_allocation *ptr_array);
void	*free_all(t_allocation *ptr_array);
void	*free_specific(\
	t_allocation *ptr_array, const void *ptr);
size_t	get_allocation_count(t_allocation *ptr_array);
int		add_to_tracking(\
	t_allocation *ptr_array, void *user_ptr, size_t count, size_t size);
void	*free_list(t_allocation *ptr_array, void **double_ptr, int count);
void	*free_one(t_allocation *ptr_array, const void *ptr);
void	*error_cleanup_sa(t_allocation *ptr_array);

/**
 * @brief Utility functions
 */
void	*ft_memset_sa(void *b, int c, size_t len);
void	*ft_calloc_sa(size_t count, size_t size);
void	*ft_memcpy_sa(void *dst, const void *src, size_t n);
void	ft_putstr_fd_sa(char *s, int fd);
void	ft_puthex_fd_sa(unsigned long n, int fd);

#endif /* FT_SAFE_ALLOCATE_H */