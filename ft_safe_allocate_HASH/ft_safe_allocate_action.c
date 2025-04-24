/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate_action.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:46:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/04/24 14:59:34 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_safe_allocate.h"

/**
 * @brief Reallocates memory for a given pointer by allocating new memory,
 *        copying the old data, and freeing the old pointer.
 * 
 * @param size An array of two size_t values representing the number of elements
 *             and size of each element.
 * @param ptr_array The tracking array holding allocated memory blocks.
 * @param ptr The pointer to be reallocated.
 * 
 * @return void* A new memory pointer on success, NULL on failure.
 */
void	*realloc_ptr(size_t size[2], t_allocation *ptr_array, void *ptr)
{
	void	*new_ptr;

	new_ptr = allocate_ptr(size, ptr_array);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		ft_memcpy(new_ptr, ptr, size[0] * size[1]);
		free_specific(ptr_array, ptr, NULL);
	}
	return (new_ptr);
}

/**
 * @brief Returns the number of active (non-NULL) allocations in the tracking
 *        array.
 * 
 * @param ptr_array The tracking array holding allocated memory blocks.
 * 
 * @return int The number of active allocations.
 */
int	get_allocation_count(t_allocation *ptr_array)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (i < HASH_TABLE_SIZE)
	{
		if (ptr_array[i].user_ptr != NULL)
			count++;
		i++;
	}
	if (count > HASH_TABLE_SIZE * 0.9)
		ft_putstr_fd(WARN_NEAR_ALLOC_LIMIT, STDOUT_FILENO);
	return (count);
}

/**
 * @brief Handles different memory freeing scenarios based on input parameters
 * 
 * @param ptr_array Array of allocation records
 * @param ptr Single pointer to be freed (optional)
 * @param double_ptr Array of pointers to be freed (optional)
 * @return void* Always returns NULL
 */
void *free_specific(t_allocation *ptr_array, const void *ptr, void **double_ptr)
{
	if (!ptr && !double_ptr)
	{
		ft_putstr_fd(WARN_FREE_NULL_PTR, STDERR_FILENO);
		return (NULL);
	}
	if (ptr && double_ptr)
		return (NULL);
	if (ptr && MEMORY_FENCING)
		return (free_one_memfen(ptr_array, ptr));
	if (ptr && !MEMORY_FENCING)
		return (free_one(ptr_array, ptr));
	if (double_ptr)
		return (free_list(ptr_array, double_ptr));
	return (NULL);
}

/**
 * @brief Frees all memory blocks stored in the tracking array and resets each
 *        slot.
 * 
 * @param ptr_array The tracking array holding allocated memory blocks.
 * 
 * @return void* Always returns NULL.
 */
void	*free_all(t_allocation *ptr_array)
{
	int	i;

	i = 0;
	while (i < HASH_TABLE_SIZE)
	{
		if (ptr_array[i].user_ptr)
		{
			if (MEMORY_FENCING)
			{
				check_memfen(ptr_array[i].user_ptr, ptr_array[i].size);
				free(ptr_array[i].original_ptr);
			}
			else
				free(ptr_array[i].user_ptr);
			ft_memset(&ptr_array[i], 0, sizeof(t_allocation));
		}
		i++;
	}
	return (NULL);
}

/**
 * @brief Allocates memory using ft_calloc and adds it to the tracking array.
 *        If allocation fails or tracking fails, all memory is freed.
 * 
 * @param size An array of two size_t values representing the number of elements
 *             and size of each element.
 * @param ptr_array The tracking array to store the allocation information.
 * 
 * @return void* The allocated memory pointer on success, NULL on failure.
 */
void	*allocate_ptr(size_t size[2], t_allocation *ptr_array)
{
	void	*user_ptr;
	void	*original_ptr;

	if (MEMORY_FENCING)
	{
		original_ptr = ft_calloc(1, (size[0] * size[1]) + (GUARD_SIZE * 2));
		if (!original_ptr)
			return (free_all(ptr_array));
		user_ptr = setup_memfen(original_ptr, size[0] * size[1]);
		if (add_to_tracking(ptr_array, original_ptr, user_ptr, size) == ERROR)
			return (free(original_ptr), free_all(ptr_array));
	}
	else if (!MEMORY_FENCING)
	{
		user_ptr = ft_calloc(size[0], size[1]);
		if (!user_ptr)
			return (free_all(ptr_array));
		if (add_to_tracking(ptr_array, NULL, user_ptr, size) == ERROR)
			return (free(user_ptr), free_all(ptr_array));
	}
	return (user_ptr);
}
