/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:11:58 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/26 16:35:58 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_safe_allocate.h"

void *error_cleanup(t_allocation_node **head)
{
	free_all(head);
	ft_putstr_fd_sa(PROMPT, 2);
	ft_putstr_fd_sa("\e[1;33m", 2);
	ft_putstr_fd_sa("ALLOCATION", 2);
	ft_putstr_fd_sa("\e[90m", 2);
	ft_putstr_fd_sa(": ", 2);
	ft_putstr_fd_sa("malloc failed", 2);
	ft_putstr_fd_sa("\e[0m\n", 2);
	exit(1);
	return (NULL);
}

void *ft_safe_allocate(size_t count, size_t size, t_action action, void *ptr)
{
	static t_allocation_node	*allocations = NULL;
	void						*result;

	result = NULL;
	if (action == ALLOCATE)
		result = allocate_ptr(&allocations, count, size);
	else if (action == FREE_ALL)
		result = free_all(&allocations);
	else if (action == FREE_ONE)
		result = free_specific(&allocations, ptr);
	else if (action == GET_USAGE)
		result = (void *)(uintptr_t)get_allocation_count(&allocations);
	else if (action == ADD_TO_TRACK)
		result = add_to_tracking(&allocations, ptr, count * size);
	return (result);
}
