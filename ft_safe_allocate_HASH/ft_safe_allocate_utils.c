/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:25:38 by mait-you          #+#    #+#             */
/*   Updated: 2025/04/24 14:44:13 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_safe_allocate.h"

/**
 * @brief Fills a block of memory with a specific byte value.
 * 
 * @param b Pointer to the block of memory to fill.
 * @param c The byte value to set.
 * @param len Number of bytes to set.
 * 
 * @return void* The original pointer 'b'.
 */
void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)b;
	while (len-- > 0)
		*(ptr++) = (unsigned char)c;
	return (b);
}

/**
 * @brief Allocates and zeroes memory for an array of elements.
 * 
 * @param count Number of elements to allocate.
 * @param size Size of each element.
 * 
 * @return void* Pointer to the allocated and zero-initialized memory block,
 *         or NULL on failure or overflow.
 */
void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (size != 0 && count > SIZE_MAX / size)
		return (NULL);
	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_memset(ptr, 0, count * size);
	return (ptr);
}

/**
 * @brief Copies 'n' bytes from source to destination.
 * 
 * @param dst Destination buffer.
 * @param src Source buffer.
 * @param n Number of bytes to copy.
 * 
 * @return void* Pointer to the destination buffer.
 */
void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	char			*ptr;

	if (!src && !dst)
		return (0);
	i = 0;
	ptr = (char *)dst;
	while (i < n)
	{
		*(ptr++) = *((const char *)src++);
		i++;
	}
	return (dst);
}

/**
 * @brief Writes a string followed by a newline to the given file descriptor.
 * 
 * @param s The string to write.
 * @param fd The file descriptor.
 */
void	ft_putstr_fd(char *s, int fd)
{
	if (!s || fd == -1)
		return ;
	while (*s)
		write(fd, s++, 1);
}

/**
 * @brief Writes an integer in lowercase hexadecimal to the given file
 *        descriptor.
 * 
 * @param n The integer to write.
 * @param fd The file descriptor.
 */
void	ft_puthex_fd(unsigned int n, int fd)
{
	char	*b;

	b = "0123456789abcdef";
	if (fd == -1)
		return ;
	if (n >= 16)
		ft_puthex_fd(n / 16, fd);
	write(fd, &b[n % 16], 1);
}
