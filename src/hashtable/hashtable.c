/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 14:58:27 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/09 18:48:39 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashtable.h"
#include "libft.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

t_hashtable	*hashtable_create(void)
{
	t_hashtable			*table;

	table = ft_calloc(1, sizeof(t_hashtable));
	if (!table)
		return (NULL);
	table->capacity = INIT_CAPACITY;
	table->items = ft_calloc(table->capacity, sizeof(t_list *));
	if (!table->items)
	{
		free(table);
		return (NULL);
	}
	return (table);
}

void	hashtable_destroy(t_hashtable *table)
{
	size_t				i;

	i = 0;
	while (i < table->capacity)
	{
		ft_lstclear(&table->items[i], hashtable_destroy_item);
		i++;
	}
	free(table->items);
	free(table);
}

uint64_t	hashtable_key(const char *key)
{
	uint64_t	hash;
	const char	*p;

	hash = FNV_OFFSET;
	p = key;
	while (*p)
	{
		hash ^= (uint64_t)(unsigned char)(*p);
		hash *= FNV_PRIME;
		p++;
	}
	return (hash);
}

void	*hashtable_get(t_hashtable *table, const char *key)
{
	t_list				*node;
	uint64_t			hash;
	size_t				index;

	hash = hashtable_key(key);
	index = (size_t)(hash & (uint64_t)(table->capacity - 1));
	node = table->items[index];
	while (node)
	{
		if (ft_strcmp(key, ((t_hashtable_item *)node->content)->key) == 0)
			return (((t_hashtable_item *)node->content)->value);
		node = node->next;
	}
	return (NULL);
}

const char	*hashtable_set(t_hashtable *table, const char *key, void *value)
{
	if (!value)
		return (NULL);
	if (table->len >= table->capacity / 2)
	{
		if (!hashtable_expand(table))
			return (NULL);
	}
	return (hashtable_set_item(table, key, value));
}
