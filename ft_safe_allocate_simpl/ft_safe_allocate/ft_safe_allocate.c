/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:46:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/15 08:59:53 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_safe_allocate.h"

size_t	hash_ptr(const void *ptr)
{
	uintptr_t key;
	
	key = (uintptr_t)ptr;
	key ^= (key >> 33);
	key *= 0xff51afd7ed558ccd;
	key ^= (key >> 33);
	key *= 0xc4ceb9fe1a85ec53;
	key ^= (key >> 33);
	return ((size_t)(key % HASH_TABLE_SIZE));
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
	else if (action == ADD_TO_TRACK)
	{
		add_to_tracking(ptr_array, ptr, size);
		user_ptr = ptr;
	}
	return (user_ptr);
}
