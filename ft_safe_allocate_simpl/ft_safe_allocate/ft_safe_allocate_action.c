/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate_action.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:46:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/15 08:56:20 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_safe_allocate.h"

void	*allocate_ptr(size_t *size, t_allocation *ptr_array)
{
	void	*user_ptr;

	user_ptr = ft_calloc_sa(size[0], size[1]);
	if (!user_ptr)
		return (error_cleanup_sa(ptr_array));
	if (add_to_tracking(ptr_array, user_ptr, size) == ERROR)
		return (free(user_ptr), error_cleanup_sa(ptr_array));
	return (user_ptr);
}

void	*free_all(t_allocation *ptr_array)
{
	int	i;

	i = 0;
	while (i < HASH_TABLE_SIZE)
	{
		if (ptr_array[i].user_ptr)
		{
			free(ptr_array[i].user_ptr);
			ft_memset_sa(&ptr_array[i], 0, sizeof(t_allocation));
		}
		i++;
	}
	return (NULL);
}

size_t	get_allocation_count(t_allocation *ptr_array)
{
	int		count;
	size_t	bytes;
	int		i;

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

int	add_to_tracking(
	t_allocation *ptr_array, void *user_ptr, size_t *size)
{
	size_t	hash;
	int		i;
	size_t	start_hash;

	i = 0;
	hash = hash_ptr(user_ptr);
	start_hash = hash;
	while (i < HASH_TABLE_SIZE)
	{
		if (ptr_array[hash].user_ptr == NULL)
		{
			ptr_array[hash].user_ptr = user_ptr;
			if (size)
				ptr_array[hash].size = size[0] * size[1];
			return (SUCCESS);
		}
		hash = (hash + 1) % HASH_TABLE_SIZE;
		i++;
		if (hash == start_hash)
			break;
	}
	ft_putstr_fd_sa(ERR_ALLOC_TRACK_LIMIT, STDERR_FILENO);
	return (ERROR);
}
