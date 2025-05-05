/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_fencing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:46:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/04 16:39:33 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_safe_allocate.h"

int	add_to_tracking(
	t_allocation *ptr_array, void *original_ptr, void *user_ptr, size_t *size)
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
			if (size)
				ptr_array[hash].size = size[0] * size[1];
			return (SUCCESS);
		}
		hash = (hash + 1) % HASH_TABLE_SIZE;
		i++;
	}
	ft_putstr_fd_sa(ERR_ALLOC_TRACK_LIMIT, STDERR_FILENO);
	return (ERROR);
}

void	*setup_memfen(void *ptr, size_t total_size)
{
	if (!ptr)
		return (NULL);
	ft_memset_sa(ptr, GUARD_PATTERN, GUARD_SIZE);
	ft_memset_sa(
		(unsigned char *)ptr + GUARD_SIZE + total_size,
		GUARD_PATTERN,
		GUARD_SIZE);
	return ((unsigned char *)ptr + GUARD_SIZE);
}

static int	check_guard(
	unsigned char *guard, void *user_ptr, const char *error_msg)
{
	int	i;

	i = 0;
	while (i < GUARD_SIZE)
	{
		if (guard[i] != GUARD_PATTERN)
		{
			ft_putstr_fd_sa((char *)error_msg, STDERR_FILENO);
			ft_puthex_fd_sa((unsigned long)user_ptr, STDERR_FILENO);
			write(STDERR_FILENO, "\n", 1);
			return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

int	check_memfen(void *user_ptr, size_t total_size)
{
	unsigned char	*start_guard;
	unsigned char	*end_guard;

	start_guard = (unsigned char *)user_ptr - GUARD_SIZE;
	end_guard = (unsigned char *)user_ptr + total_size;
	if (check_guard(start_guard, user_ptr, ERR_CORRUPTION_START) == ERROR)
		return (ERROR);
	if (check_guard(end_guard, user_ptr, ERR_CORRUPTION_END) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
