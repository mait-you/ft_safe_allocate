/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate_action.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:46:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/06 14:28:16 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_safe_allocate.h"

// void    *realloc_ptr(
// 	size_t *size, t_allocation *ptr_array, void *ptr, t_action action)
// {
// 	void    *new_ptr;

// 	new_ptr = allocate_ptr((size_t[2]){size[0] , 1}, ptr_array);
// 	if (!new_ptr)
// 		return (NULL);
// 	if (ptr)
// 	{
// 		ft_memcpy_sa(new_ptr, ptr, size[1]);
// 		if (action == REALLOC)
// 			free_specific(ptr_array, ptr, NULL, 0);
// 		else if (action == ADD_TO_TRACK)
// 			free(ptr);
// 	}
// 	return (new_ptr);
// }

void    *realloc_ptr(
	size_t *size, t_allocation *ptr_array, void *ptr, t_action action)
{
	void    *new_ptr;

	if (!ptr)
		return (allocate_ptr((size_t[2]){size[0], 1}, ptr_array));
	if (size[0] == 0)
		return (free_specific(ptr_array, ptr, NULL, 0), NULL);
	new_ptr = allocate_ptr((size_t[2]){size[0], 1}, ptr_array);
	if (!new_ptr)
		return (NULL);
	if (ptr && size[1] > 0)
		ft_memcpy_sa(new_ptr, ptr, size[1]);
	if (action == REALLOC)
		free_specific(ptr_array, ptr, NULL, 0);
	else if (action == ADD_TO_TRACK)
		free(ptr);
	return (new_ptr);
}

size_t	get_allocation_count(t_allocation *ptr_array)
{
	int		count;
	size_t	bytes;
	int	i;

	count = 0;
	i = 0;
	bytes = 0;
	while (i < HASH_TABLE_SIZE)
	{
		if (ptr_array[i].user_ptr != NULL)
		{
			count++;
			bytes += ptr_array[i].size;
		}
		i++;
	}
	if (count > HASH_TABLE_SIZE * 0.9)
		ft_putstr_fd_sa(WARN_NEAR_ALLOC_LIMIT, STDOUT_FILENO);
	return (bytes);
}

void	*free_specific(
	t_allocation *ptr_array, const void *ptr, void **double_ptr, size_t *size)
{
	int	count;

	count = 0;
	if (!ptr && !double_ptr)
		return (ft_putstr_fd_sa(WARN_FREE_NULL_PTR, STDERR_FILENO), NULL);
	if (ptr && double_ptr)
		return (ft_putstr_fd_sa(WARN_BOTH_PTR, STDERR_FILENO), NULL);
	if (ptr)
	{
		if (MEMORY_FENCING)
			return (free_one_memfen(ptr_array, ptr));
		else if (!MEMORY_FENCING)
			return (free_one(ptr_array, ptr));
	}
	if (size)
		count = *size;
	if (double_ptr)
		return (free_list(ptr_array, double_ptr, count));
	return (NULL);
}

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
			ft_memset_sa(&ptr_array[i], 0, sizeof(t_allocation));
		}
		i++;
	}
	return (NULL);
}

void	*allocate_ptr(size_t *size, t_allocation *ptr_array)
{
	void	*user_ptr;
	void	*original_ptr;

	if (MEMORY_FENCING)
	{
		original_ptr = ft_calloc_sa(1, (size[0] * size[1]) + (GUARD_SIZE * 2));
		if (!original_ptr)
			return (error_cleanup_sa(ptr_array));
		user_ptr = setup_memfen(original_ptr, size[0] * size[1]);
		if (add_to_tracking(ptr_array, original_ptr, user_ptr, size) == ERROR)
			return (free(original_ptr), error_cleanup_sa(ptr_array));
	}
	else if (!MEMORY_FENCING)
	{
		user_ptr = ft_calloc_sa(size[0], size[1]);
		if (!user_ptr)
			return (error_cleanup_sa(ptr_array));
		if (add_to_tracking(ptr_array, NULL, user_ptr, size) == ERROR)
			return (free(user_ptr), error_cleanup_sa(ptr_array));
	}
	return (user_ptr);
}
