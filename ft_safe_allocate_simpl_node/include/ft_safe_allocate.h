/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:44:47 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/20 10:00:00 by mait-you         ###   ########.fr       */
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
 * @brief Warning threshold for allocation count
 * A warning will be issued when allocation count exceeds this value
 */
# define ALLOC_WARNING_THRESHOLD 1000

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
typedef enum e_action			t_action;
typedef struct s_alloc_node     t_alloc_node;
typedef struct s_alloc_list     t_alloc_list;

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
 * @brief Node structure for linked list of allocations
 * 
 * @param user_ptr      Pointer provided to user
 * @param size          Size of allocated memory
 * @param next          Pointer to next node in the list
 */
struct s_alloc_node
{
    void                *user_ptr;
    size_t              size;
    struct s_alloc_node *next;
};

/**
 * @brief Linked list structure to track memory allocations
 * 
 * @param head          Pointer to first node in the list
 * @param count         Number of allocations currently tracked
 * @param total_size    Total bytes allocated
 */
struct s_alloc_list
{
    t_alloc_node *head;
    size_t       count;
    size_t       total_size;
};

/**
 * @brief Action enum for ft_safe_allocate function
 */
enum e_action
{
	ALLOCATE,			/* Allocate new memory */
	FREE_ALL,			/* Free all tracked allocations */
	FREE_ONE,			/* Free a specific allocation */
	GET_USAGE,			/* Get count of current allocations */
	ADD_TO_TRACK,		/* Add externally allocated memory to tracking */
};

/* ************************************************************************** */
/* 							Function Prototypes                               */
/* ************************************************************************** */

/**
 * @brief Safe memory management function with multiple operations
 *
 * This function handles memory allocation, deallocation, and tracking.
 * It can allocate new memory, free specific or all allocations, get usage 
 * statistics, and add external allocations to tracking.
 *
 * @param size Pointer to size info | interpretation depends on action:
 *        - For ALLOCATE: size[0]=count, size[1]=element size
 *        - For FREE_ONE: size[0]=element count of @double_ptr
 *        - For other actions: Can be NULL
 * @param action Operation to perform (ALLOCATE, FREE_ALL, FREE_ONE,
 *         GET_USAGE, ADD_TO_TRACK)
 * @param ptr Pointer to free (for FREE_ONE)
 * @param double_ptr Array of pointers to free (optional with FREE_ONE)
 *
 * @return For ALLOCATE: Allocated pointer
 *         For GET_USAGE: Cast (void *)(uintptr_t) of allocation count
 *         For FREE_ONE/FREE_ALL: NULL
 *         On error: NULL
 */
void	*ft_safe_allocate(\
	size_t *size, t_action action, void *ptr, void **double_ptr);

/**
 * @brief Creates a new allocation node
 * 
 * @param user_ptr      Pointer to allocated memory
 * @param size          Size of allocated memory
 * @return t_alloc_node* New node or NULL if creation failed
 */
t_alloc_node *create_alloc_node(void *user_ptr, size_t size);

/**
 * @brief Finds node containing the given pointer
 * 
 * @param list          The allocation list
 * @param ptr           Pointer to search for
 * @return t_alloc_node* Node containing the pointer, or NULL if not found
 */
t_alloc_node *find_alloc_node(t_alloc_list *list, const void *ptr);

/**
 * @brief Adds a node to the allocation list
 * 
 * @param list          The allocation list
 * @param user_ptr      Pointer to allocated memory
 * @param size          Size of allocated memory
 * @return int          SUCCESS if added successfully, ERROR otherwise
 */
int add_alloc_node(t_alloc_list *list, void *user_ptr, size_t size);

/**
 * @brief Removes and frees a node from the allocation list
 * 
 * @param list          The allocation list
 * @param ptr           Pointer to allocated memory to remove
 * @return void*        Always returns NULL
 */
void *remove_alloc_node(t_alloc_list *list, const void *ptr);

/**
 * @brief Frees all nodes in the allocation list
 * 
 * @param list          The allocation list
 * @return void*        Always returns NULL
 */
void *free_alloc_list(t_alloc_list *list);

/**
 *  Action functions
 */

/**
 * @brief Allocates memory and adds it to the tracking system
 *
 * This function allocates memory of the specified size and adds it to
 * the allocation tracking list.
 *
 * @param size Pointer to size array: size[0]=count, size[1]=element size
 * @param list The allocation tracking list
 *
 * @return Pointer to the allocated memory, or NULL on failure
 */
void	*allocate_ptr(size_t *size, t_alloc_list *list);

/**
 * @brief Frees all tracked allocations
 *
 * This function iterates through the tracking list and frees all
 * non-NULL pointers that have been allocated by the system.
 *
 * @param list The allocation tracking list
 *
 * @return Always returns NULL to indicate all memory has been freed
 */
void	*free_all(t_alloc_list *list);

/**
 * @brief Handles specific free operations based on provided parameters
 *
 * This function serves as a router for different types of free operations
 * based on the parameters provided. It can free a single pointer or an array.
 *
 * @param list The allocation tracking list
 * @param ptr The pointer to free
 * @param double_ptr Optional array of pointers to free
 * @param size Optional size information
 *
 * @return Always returns NULL to indicate the memory has been freed
 */
void	*free_specific(\
	t_alloc_list *list, const void *ptr, void **double_ptr, size_t *size);

/**
 * @brief Gets the number of active allocations in the tracking system
 *
 * This function returns the total size of allocations in the tracking list.
 *
 * @param list The allocation tracking list
 *
 * @return Total size of active allocations
 */
size_t	get_allocation_count(t_alloc_list *list);

/**
 *  	cleanup functions
 */

/**
 * @brief Frees an array of pointers tracked in the allocation system
 *
 * This function frees a list of pointers stored in a double pointer array.
 * It will free 'count' number of pointers from the array.
 *
 * @param list The allocation tracking list
 * @param double_ptr Array of pointers to free
 * @param count Number of pointers to free from the array
 *
 * @return Always returns NULL to indicate the memory has been freed
 */
void	*free_list(t_alloc_list *list, void **double_ptr, int count);

/**
 * @brief Frees a single pointer tracked in the allocation system
 *
 * This function frees a pointer.
 * It searches for the pointer in the tracking list and frees it if found.
 *
 * @param list The allocation tracking list
 * @param ptr The pointer to free
 *
 * @return Always returns NULL to indicate the memory has been freed
 */
void	*free_one(t_alloc_list *list, const void *ptr);

/**
 * @brief Performs cleanup operations when an error occurs
 * 
 * @param list The allocation tracking list
 * @return void* Returns NULL to indicate error condition
 */
void	*error_cleanup_sa(t_alloc_list *list);

/**
 * @brief Utility functions
 */
void	*ft_memset_sa(void *b, int c, size_t len);
void	*ft_calloc_sa(size_t count, size_t size);
void	*ft_memcpy_sa(void *dst, const void *src, size_t n);
void	ft_putstr_fd_sa(char *s, int fd);
void	ft_puthex_fd_sa(unsigned long n, int fd);

#endif /* FT_SAFE_ALLOCATE_H */