/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:46:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/20 10:00:00 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_safe_allocate.h"

t_alloc_node *create_alloc_node(void *user_ptr, size_t size)
{
    t_alloc_node *new_node;
    
    new_node = (t_alloc_node *)malloc(sizeof(t_alloc_node));
    if (!new_node)
        return (NULL);
    new_node->user_ptr = user_ptr;
    new_node->size = size;
    new_node->next = NULL;
    return (new_node);
}

t_alloc_node *find_alloc_node(t_alloc_list *list, const void *ptr)
{
    t_alloc_node *current;
    
    if (!list || !ptr)
        return (NULL);
    current = list->head;
    while (current)
    {
        if (current->user_ptr == ptr)
            return (current);
        current = current->next;
    }
    return (NULL);
}

void	*ft_safe_allocate(
	size_t *size,
	t_action action,
	void *ptr,
	void **double_ptr
	)
{
    static t_alloc_list alloc_list = {NULL, 0, 0};
    void                *user_ptr;

    user_ptr = NULL;
    if (action == ALLOCATE)
        user_ptr = allocate_ptr(size, &alloc_list);
    else if (action == FREE_ALL)
        user_ptr = free_all(&alloc_list);
    else if (action == FREE_ONE)
        user_ptr = free_specific(&alloc_list, ptr, double_ptr, size);
    else if (action == GET_USAGE)
        user_ptr = (void *)(uintptr_t)get_allocation_count(&alloc_list);
    else if (action == ADD_TO_TRACK)
    {
        if (add_alloc_node(&alloc_list, ptr, size ? size[0] * size[1] : 0) == SUCCESS)
            user_ptr = ptr;
    }
    return (user_ptr);
}