/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_allocate_action.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:35:01 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/26 16:35:50 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_safe_allocate.h"

void *allocate_ptr(t_allocation_node **head, size_t count, size_t size)
{
	void *ptr;

	ptr = ft_calloc_sa(count, size);
	if (!ptr)
		return (error_cleanup(head));
	if (add_to_tracking(head, ptr, count * size) == NULL)
	{
		free(ptr);
		return (error_cleanup(head));
	}
	return (ptr);
}

void *free_all(t_allocation_node **head)
{
	t_allocation_node	*current;
	t_allocation_node	*next;

	if (!head || !*head)
		return (NULL);
	current = *head;
	while (current)
	{
		next = current->next;
		if (current->ptr)
			free(current->ptr);
		free(current);
		current = next;
	}
	*head = NULL;
	return (NULL);
}

void *free_specific(t_allocation_node **head, const void *ptr)
{
	t_allocation_node	*current;
	t_allocation_node	*prev;

	if (!ptr)
	{
		ft_putstr_fd_sa(WARN_FREE_NULL_PTR, STDERR_FILENO);
		return (NULL);
	}
	if (!head || !*head)
		return (NULL);
	current = *head;
	prev = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				*head = current->next;
			free(current->ptr);
			free(current);
			return (NULL);
		}
		prev = current;
		current = current->next;
	}
	return (NULL);
}

size_t get_allocation_count(t_allocation_node **head)
{
	t_allocation_node *current;
	size_t total_bytes;

	if (!head || !*head)
		return (0);
	current = *head;
	total_bytes = 0;
	while (current)
	{
		total_bytes += current->size;
		current = current->next;
	}
	return (total_bytes);
}

void *add_to_tracking(t_allocation_node **head, void *ptr, size_t size)
{
	t_allocation_node *new_node;

	if (!ptr || !head)
		return (NULL);
	new_node = malloc(sizeof(t_allocation_node));
	if (!new_node)
		return (NULL);
	new_node->ptr = ptr;
	new_node->size = size;
	new_node->next = *head;
	*head = new_node;
	return (ptr);
}
