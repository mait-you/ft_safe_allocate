/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:25:38 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/26 16:35:54 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_safe_allocate.h"

void	*ft_memset_sa(void *b, int c, size_t len)
{
	unsigned char	*ptr;

	if (!b)
		return (NULL);
	ptr = (unsigned char *)b;
	while (len-- > 0)
		*(ptr++) = (unsigned char)c;
	return (b);
}

void	*ft_calloc_sa(size_t count, size_t size)
{
	void	*ptr;

	if (size != 0 && count > SIZE_MAX / size)
		return (NULL);
	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_memset_sa(ptr, 0, count * size);
	return (ptr);
}

void	*ft_memcpy_sa(void *dst, const void *src, size_t n)
{
	size_t			i;
	char			*d;
	const char		*s;

	if (!src && !dst)
		return (NULL);
	if (!dst || !src)
		return (dst);
	
	d = (char *)dst;
	s = (const char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

void	ft_putstr_fd_sa(char *s, int fd)
{
	size_t	len;
	
	if (!s || fd < 0)
		return ;
	len = 0;
	while (s[len])
		len++;
	write(fd, s, len);
}

void	ft_puthex_fd_sa(unsigned long n, int fd)
{
	char	*base;

	base = "0123456789abcdef";
	if (fd < 0)
		return ;
	if (n >= 16)
		ft_puthex_fd_sa(n / 16, fd);
	write(fd, &base[n % 16], 1);
}
