/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:46:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/03 14:18:08 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_safe_allocate.h"

size_t	hash_ptr(const void *ptr)
{
	return ((uintptr_t)ptr % HASH_TABLE_SIZE);
}

void	*ft_safe_allocate(
	size_t *size,
	t_action action,
	void *ptr,
	void **double_ptr
	)
{
	static t_allocation		ptr_array[HASH_TABLE_SIZE];
	void					*user_ptr;

	user_ptr = NULL;
	if (action == ALLOCATE)
		user_ptr = allocate_ptr(size, ptr_array);
	else if (action == FREE_ALL)
		user_ptr = free_all(ptr_array);
	else if (action == FREE_ONE)
		user_ptr = free_specific(ptr_array, ptr, double_ptr, size);
	else if (action == GET_USAGE)
		user_ptr = (void *)(uintptr_t)get_allocation_count(ptr_array);
	else if (action == REALLOC)
		user_ptr = realloc_ptr(size, ptr_array, ptr, REALLOC);
	else if (action == ADD_TO_TRACK)
	{
		add_to_tracking(ptr_array, NULL, ptr, size);
		user_ptr = ptr;
	}
	return (user_ptr);
}
