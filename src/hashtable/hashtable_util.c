/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_util.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 12:39:12 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/11 14:05:28 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashtable.h"
#include "libft.h"
#include <stdlib.h>

static int	remove_hashtable_node(t_hashtable *table, t_list **bucket,
		t_list *prev, t_list *node)
{
	if (prev == NULL)
		*bucket = node->next;
	else
		prev->next = node->next;
	ft_lstdelone(node, hashtable_destroy_item);
	table->len--;
	return (1);
}

int	hashtable_delete(t_hashtable *table, const char *key)
{
	t_list	*node;
	t_list	*prev;
	size_t	index;

	if (table->len == 0)
		return (0);
	index = hashtable_key(key) & (table->capacity - 1);
	prev = NULL;
	node = table->items[index];
	while (node)
	{
		if (ft_strcmp(key, ((t_hashtable_item *)node->content)->key) == 0)
			return (remove_hashtable_node(table, &table->items[index], prev,
					node));
		prev = node;
		node = node->next;
	}
	return (0);
}

static void	rehash_node(t_hashtable *table, t_list *node)
{
	uint64_t	hash;
	size_t		new_index;

	hash = hashtable_key(((t_hashtable_item *)node->content)->key);
	new_index = (size_t)(hash & (uint64_t)(table->capacity - 1));
	node->next = table->items[new_index];
	table->items[new_index] = node;
}

static void	rehash_all_items(t_hashtable *table, t_list **old_items,
		size_t old_capacity)
{
	t_list	*node;
	t_list	*next;
	size_t	i;

	i = -1;
	while (++i < old_capacity)
	{
		node = old_items[i];
		while (node)
		{
			next = node->next;
			rehash_node(table, node);
			node = next;
		}
	}
}

int	hashtable_expand(t_hashtable *table)
{
	t_list	**old_items;
	size_t	old_capacity;

	old_items = table->items;
	old_capacity = table->capacity;
	table->items = ft_calloc(table->capacity * 2, sizeof(t_list *));
	if (!table->items)
	{
		table->items = old_items;
		return (0);
	}
	table->capacity *= 2;
	rehash_all_items(table, old_items, old_capacity);
	free(old_items);
	return (1);
}
