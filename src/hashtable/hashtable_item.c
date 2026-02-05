/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_item.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 20:10:16 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/09 20:19:54 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashtable.h"
#include <stdlib.h>

t_hashtable_item	*hashtable_create_item(const char *key, void *value)
{
	t_hashtable_item	*item;

	item = ft_calloc(1, sizeof(t_hashtable_item));
	if (!item)
		return (NULL);
	item->key = ft_strdup(key);
	if (!item->key)
	{
		free(item);
		return (NULL);
	}
	item->value = value;
	return (item);
}

void	hashtable_destroy_item(void *item)
{
	t_hashtable_item	*item_val;

	item_val = (t_hashtable_item *)item;
	if (item_val)
	{
		if (item_val->key)
			free((void *)item_val->key);
		if (item_val->value)
			free(item_val->value);
	}
	free(item_val);
}

static const char	*update_existing(t_list *node, void *value)
{
	t_hashtable_item	*item;

	item = (t_hashtable_item *)node->content;
	if (item->value)
		free(item->value);
	item->value = value;
	return (item->value);
}

const char	*hashtable_set_item(t_hashtable *table, const char *key,
		void *value)
{
	t_list				*node;
	t_hashtable_item	*new_item;
	t_list				*new_node;
	size_t				index;

	index = hashtable_key(key) & (table->capacity - 1);
	node = table->items[index];
	while (node)
	{
		if (ft_strcmp(key, ((t_hashtable_item *)node->content)->key) == 0)
			return (update_existing(node, value));
		node = node->next;
	}
	new_item = hashtable_create_item(key, value);
	if (!new_item)
		return (NULL);
	new_node = ft_lstnew(new_item);
	if (!new_node)
	{
		hashtable_destroy_item(new_item);
		return (NULL);
	}
	ft_lstadd_back(&table->items[index], new_node);
	table->len++;
	return (key);
}
