/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memfen.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:46:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/04/24 10:53:08 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_safe_allocate.h"

/**
 * @brief Sets up memory guards (fencing) before and after the user memory.
 * 
 * @param ptr The original allocated pointer.
 * @param total_size The size of the user-usable memory block.
 * 
 * @return void* Pointer to the user-usable memory (after the first guard).
 */
static void	*setup_memfen(void *ptr, size_t total_size)
{
	if (!ptr)
		return (NULL);
	ft_memset(ptr, GUARD_PATTERN, GUARD_SIZE);
	ft_memset(
		(unsigned char *)ptr + GUARD_SIZE + total_size,
		GUARD_PATTERN,
		GUARD_SIZE);
	return ((unsigned char *)ptr + GUARD_SIZE);
}

/**
 * @brief Checks the guard regions before and after the user memory for 
 *        corruption.
 * 
 * @param user_ptr Pointer to the user-usable memory.
 * @param total_size The size of the user-usable memory block.
 * 
 * @return int 0 if no corruption, 1 if corruption is detected.
 */
static int	check_memfen(void *user_ptr, size_t total_size)
{
	unsigned char	*start_guard;
	unsigned char	*end_guard;
	int				i;

	start_guard = (unsigned char *)user_ptr - GUARD_SIZE;
	end_guard = (unsigned char *)user_ptr + total_size;
	i = 0;
	while (i < GUARD_SIZE)
	{
		if (start_guard[i] != GUARD_PATTERN)
		{
			ft_putendl_fd(ERR_CORRUPTION_START, STDERR_FILENO);
			return (ERROR);
		}
		if (end_guard[i] != GUARD_PATTERN)
		{
			ft_putendl_fd(ERR_CORRUPTION_END, STDERR_FILENO);
			return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

/**
 * @brief Frees a single memory allocation and updates the allocation table
 * 
 * @param ptr_array Array of allocation records
 * @param ptr Pointer to memory that needs to be freed
 * @return void* Always returns NULL
 */
void *free_one_memfen(t_allocation *ptr_array, const void *ptr)
{
	int		i;
	size_t	hash;
	void	*original_ptr;

	original_ptr = (void *)((unsigned char *)ptr - GUARD_SIZE);
	hash = hash_ptr(original_ptr);
	i = 0;
	while (i < HASH_TABLE_SIZE)
	{
		if (ptr_array[hash].original_ptr == original_ptr)
		{
			check_memfen(\
				ptr_array[hash].user_ptr, ptr_array[hash].size);
			free(ptr_array[hash].original_ptr);
			ft_memset(&ptr_array[hash], 0, sizeof(t_allocation));
			return (NULL);
		}
		if (ptr_array[hash].original_ptr == NULL)
			break;
		hash = (hash + 1) % HASH_TABLE_SIZE;
		i++;
	}
	free((void *)original_ptr);
	ft_putendl_fd(WARN_PTR_NOT_ALLOCATED, STDERR_FILENO);
	return (NULL);
}

/**
 * @brief Frees a list of memory allocations and the list pointer itself
 * 
 * @param ptr_array Array of allocation records
 * @param double_ptr Array of pointers to memory that needs to be freed
 * @return void* Always returns NULL
 */
void *free_list_memfen(t_allocation *ptr_array, void **double_ptr)
{
	int		i;

	i = 0;
	while (double_ptr[i])
	{
		free_one_memfen(ptr_array, double_ptr[i]);
		double_ptr[i] = NULL;
		i++;
	}
	free_one_memfen(ptr_array, double_ptr);
	return (NULL);
}

/**
 * @brief Frees a specific allocation with memory fencing validation.
 * 
 * @param ptr_array Array tracking all allocations.
 * @param ptr The user pointer to be freed.
 * 
 * @return void* Always returns NULL.
 */
void	*free_specific_memfen(
	t_allocation *ptr_array, const void *ptr, void **double_ptr)
{
	if (ptr)
		return (free_one_memfen(ptr_array, ptr));
	if (double_ptr)
		return (free_list_memfen(ptr_array, double_ptr));
	return (NULL);
}

/**
 * @brief Frees all tracked allocations with memory fencing validation.
 * 
 * @param ptr_array Array tracking all allocations.
 * 
 * @return void* Always returns NULL.
 */
void	*free_all_memfen(t_allocation *ptr_array)
{
	int	i;

	i = 0;
	while (i < HASH_TABLE_SIZE)
	{
		if (ptr_array[i].user_ptr)
		{
			check_memfen(ptr_array[i].user_ptr, ptr_array[i].size);
			free(ptr_array[i].original_ptr);
			ft_memset(&ptr_array[i], 0, sizeof(t_allocation));
		}
		i++;
	}
	return (NULL);
}

/**
 * @brief Allocates memory with fencing and tracks the allocation.
 * 
 * @param size Array with two elements: count and size of each element.
 * @param ptr_array Array used to track allocations.
 * 
 * @return void* Pointer to the user-usable memory, or NULL on failure.
 */
void	*allocate_ptr_memfen(size_t size[2], t_allocation *ptr_array)
{
	void	*original_ptr;
	void	*user_ptr;

	original_ptr = ft_calloc(1, (size[0] * size[1]) + (GUARD_SIZE * 2));
	if (!original_ptr)
	{
		free_all_memfen(ptr_array);
		return (NULL);
	}
	user_ptr = setup_memfen(original_ptr, size[0] * size[1]);
	if (add_to_tracking(ptr_array, original_ptr, user_ptr, size) == ERROR)
	{
		free(original_ptr);
		free_all_memfen(ptr_array);
		return (NULL);
	}
	return (user_ptr);
}
