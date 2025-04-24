/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate_cleanup.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:46:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/04/24 14:54:49 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_safe_allocate.h"

/**
 * @brief Adds a newly allocated pointer to the tracking array.
 * 
 * @param ptr_array The tracking array to store allocation info.
 * @param original_ptr The original pointer, if any (used for debugging or
 *        fencing).
 * @param user_ptr The actual user-accessible pointer to be tracked.
 * @param size An array of two size_t values: count and size of each element.
 * 
 * @return int SUCCESS if added, ERROR if the tracking array is full.
 */
int	add_to_tracking(
	t_allocation *ptr_array,
	void *original_ptr,
	void *user_ptr,
	size_t size[2])
{
	size_t	hash;
	int		i;

	i = 0;
	hash = hash_ptr(user_ptr);
	while (i < HASH_TABLE_SIZE)
	{
		if (ptr_array[hash].user_ptr == NULL)
		{
			ptr_array[hash].original_ptr = original_ptr;
			ptr_array[hash].user_ptr = user_ptr;
			ptr_array[hash].size = size[0] * size[1];
			return (SUCCESS);
		}
		hash = (hash + 1) % HASH_TABLE_SIZE;
		i++;
	}
	ft_putstr_fd(ERR_ALLOC_TRACK_LIMIT, STDERR_FILENO);
	return (ERROR);
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
	hash = hash_ptr(ptr);
	i = 0;
	while (i < HASH_TABLE_SIZE)
	{
		if (ptr_array[hash].original_ptr == original_ptr)
		{
			check_memfen(ptr_array[hash].user_ptr, ptr_array[hash].size);
			free(ptr_array[hash].original_ptr);
			ft_memset(&ptr_array[hash], 0, sizeof(t_allocation));
			return (NULL);
		}
		if (ptr_array[hash].original_ptr == NULL)
			break;
		hash = (hash + 1) % HASH_TABLE_SIZE;
		i++;
	}
	ft_putstr_fd(WARN_PTR_NOT_ALLOCATED, STDERR_FILENO);
	return (NULL);
}

/**
 * @brief Frees a single memory allocation and updates the allocation table
 * 
 * @param ptr_array Array of allocation records
 * @param ptr Pointer to memory that needs to be freed
 * @return void* Always returns NULL
 */
void *free_one(t_allocation *ptr_array, const void *ptr)
{
	int		i;
	size_t	hash;

	hash = hash_ptr(ptr);
	i = 0;
	while (i < HASH_TABLE_SIZE)
	{
		if (ptr_array[hash].user_ptr == ptr)
		{
			free(ptr_array[hash].user_ptr);
			ft_memset(&ptr_array[hash], 0, sizeof(t_allocation));
			return (NULL);
		}
		if (ptr_array[hash].user_ptr == NULL)
			break;
		hash = (hash + 1) % HASH_TABLE_SIZE;
		i++;
	}
	ft_putstr_fd(WARN_PTR_NOT_ALLOCATED, STDERR_FILENO);
	return (NULL);
}

/**
 * @brief Frees a list of memory allocations and the list pointer itself
 * 
 * @param ptr_array Array of allocation records
 * @param double_ptr Array of pointers to memory that needs to be freed
 * @return void* Always returns NULL
 */
void *free_list(t_allocation *ptr_array, void **double_ptr)
{
	int	i;

	i = 0;
	while (double_ptr && double_ptr[i])
	{
		if (MEMORY_FENCING)
			free_one_memfen(ptr_array, double_ptr[i]);
		else if (!MEMORY_FENCING)
			free_one(ptr_array, double_ptr[i]);
		double_ptr[i] = NULL;
		i++;
	}
	free_one(ptr_array, double_ptr);
	return (NULL);
}
