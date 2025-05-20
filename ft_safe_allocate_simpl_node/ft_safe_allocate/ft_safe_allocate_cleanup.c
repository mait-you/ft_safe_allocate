/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate_cleanup.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:46:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/20 10:00:00 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_safe_allocate.h"

void	*error_cleanup_sa(t_alloc_list *list)
{
    free_all(list);
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

void	*free_one(t_alloc_list *list, const void *ptr)
{
    if (!ptr)
        return (NULL);
    return (remove_alloc_node(list, ptr));
}

void *remove_alloc_node(t_alloc_list *list, const void *ptr)
{
    t_alloc_node *current;
    t_alloc_node *prev;
    
    if (!list || !ptr)
        return (NULL);
    
    current = list->head;
    prev = NULL;
    
    // Search for the node
    while (current && current->user_ptr != ptr)
    {
        prev = current;
        current = current->next;
    }
    
    // Node not found
    if (!current)
    {
        ft_putstr_fd_sa(WARN_PTR_NOT_ALLOCATED_1, STDERR_FILENO);
        ft_puthex_fd_sa((uintptr_t)ptr, STDERR_FILENO);
        ft_putstr_fd_sa(WARN_PTR_NOT_ALLOCATED_2, STDERR_FILENO);
        return (NULL);
    }
    
    // Update list metrics
    list->count--;
    list->total_size -= current->size;
    
    // Remove node from list
    if (prev)
        prev->next = current->next;
    else
        list->head = current->next;
    
    // Free memory and node
    free(current->user_ptr);
    free(current);
    
    return (NULL);
}

static void	*free_one_con(t_alloc_list *list, void **double_ptr)
{
    free(double_ptr);
    return (NULL);
}

void	*free_list(t_alloc_list *list, void **double_ptr, int count)
{
    int	i;

    i = -1;
    if (count == 0)
    {
        while (double_ptr && double_ptr[++i])
            remove_alloc_node(list, double_ptr[i]);
        return (free_one_con(list, double_ptr));
    }
    while (double_ptr && ++i < count)
        remove_alloc_node(list, double_ptr[i]);
    return (free_one_con(list, double_ptr));
}

void	*free_specific(
    t_alloc_list *list, const void *ptr, void **double_ptr, size_t *size)
{
    int	count;

    count = 0;
    if (!ptr && !double_ptr)
    {
        ft_putstr_fd_sa(WARN_FREE_NULL_PTR, STDERR_FILENO);
        return (NULL);
    }
    if (ptr && double_ptr)
    {
        ft_putstr_fd_sa(WARN_BOTH_PTR, STDERR_FILENO);
        return (NULL);
    }
    if (ptr)
        return (free_one(list, ptr));
    if (size)
        count = *size;
    if (double_ptr)
        return (free_list(list, double_ptr, count));
    return (NULL);
}

void *free_alloc_list(t_alloc_list *list)
{
    t_alloc_node *current;
    t_alloc_node *next;
    
    if (!list)
        return (NULL);
    
    current = list->head;
    
    // Free all nodes in the list
    while (current)
    {
        next = current->next;
        free(current->user_ptr);
        free(current);
        current = next;
    }
    
    // Reset list state
    list->head = NULL;
    list->count = 0;
    list->total_size = 0;
    
    return (NULL);
}
