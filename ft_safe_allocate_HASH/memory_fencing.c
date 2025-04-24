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
void	*setup_memfen(void *ptr, size_t total_size)
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
int	check_memfen(void *user_ptr, size_t total_size) 
{
	unsigned char	*start_guard;
	unsigned char	*end_guard;
	const char		*error_msg;
	int				i;

	start_guard = (unsigned char *)user_ptr - GUARD_SIZE;
	end_guard = (unsigned char *)user_ptr + total_size;
	i = 0;
	while (i++ < GUARD_SIZE)
	{
		if (start_guard[i] != GUARD_PATTERN)
			error_msg = ERR_CORRUPTION_START;
		else if (end_guard[i] != GUARD_PATTERN)
			error_msg = ERR_CORRUPTION_END;
		else
			continue ;
		ft_putstr_fd((char *)error_msg, STDERR_FILENO);
		ft_puthex_fd((unsigned long)user_ptr, STDERR_FILENO);
		write(STDERR_FILENO, "\n", 1);
		return (ERROR);
	}
	return (SUCCESS);
}
