#include "../include/ft_safe_allocate.h"

void *allocate_ptr(t_allocation_node **head, size_t count, size_t size)
{
	void *ptr;

	ptr = ft_calloc_sa(count, size);
	if (!ptr)
		return (error_cleanup(head));
	if (add_to_tracking(head, ptr, count * size) == ERROR)
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

	current = *head;
	while (current)
	{
		next = current->next;
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
		return (ft_putstr_fd_sa(WARN_FREE_NULL_PTR, STDERR_FILENO), NULL);
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

	current = *head;
	total_bytes = 0;
	while (current)
	{
		total_bytes += current->size;
		current = current->next;
	}
	return (total_bytes);
}

char *add_to_tracking(t_allocation_node **head, void *ptr, size_t size)
{
	t_allocation_node *new_node;

	new_node = ft_calloc_sa(1, sizeof(t_allocation_node));
	if (!new_node)
		return (error_cleanup(head), NULL);
	new_node->ptr = ptr;
	new_node->size = size;
	new_node->next = *head;
	*head = new_node;
	return (ptr);
}
