/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate_action.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:46:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/20 10:00:00 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_safe_allocate.h"

void	*allocate_ptr(size_t *size, t_alloc_list *list)
{
    void	*user_ptr;

    user_ptr = ft_calloc_sa(size[0], size[1]);
    if (!user_ptr)
        return (error_cleanup_sa(list));
    if (add_alloc_node(list, user_ptr, size[0] * size[1]) == ERROR)
    {
        free(user_ptr);
        return (error_cleanup_sa(list));
    }
    return (user_ptr);
}

void	*free_all(t_alloc_list *list)
{
    return (free_alloc_list(list));
}

size_t	get_allocation_count(t_alloc_list *list)
{
    if (!list)
        return (0);
    return (list->total_size);
}

int add_alloc_node(t_alloc_list *list, void *user_ptr, size_t size)
{
    t_alloc_node *new_node;
    
    if (!list || !user_ptr)
        return (ERROR);
    new_node = create_alloc_node(user_ptr, size);
    if (!new_node)
    {
        ft_putstr_fd_sa(ERR_ALLOC_TRACK_LIMIT, STDERR_FILENO);
        return (ERROR);
    }
    
    // Add node to beginning of list (faster than appending)
    new_node->next = list->head;
    list->head = new_node;
    
    // Update list metrics
    list->count++;
    list->total_size += size;
    
    // Check if allocation count is getting high (warning threshold)
    if (list->count > ALLOC_WARNING_THRESHOLD)
        ft_putstr_fd_sa(WARN_NEAR_ALLOC_LIMIT, STDOUT_FILENO);
    
    return (SUCCESS);
}