/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_iterator.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 04:53:28 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/10 05:04:13 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashtable.h"
#include <stdlib.h>

t_hashtable_iterator	hashtable_iterator(t_hashtable *table)
{
	t_hashtable_iterator	it;

	it.table = table;
	it.index = 0;
	it.node = NULL;
	return (it);
}

int	hashtable_next(t_hashtable_iterator *it)
{
	t_hashtable_item	*item;

	while (it->index < it->table->capacity)
	{
		if (it->node == NULL)
			it->node = it->table->items[it->index];
		if (it->node == NULL)
		{
			it->index++;
			continue ;
		}
		item = (t_hashtable_item *)it->node->content;
		it->node = it->node->next;
		if (it->node == NULL)
			it->index++;
		it->key = item->key;
		it->value = item->value;
		return (1);
	}
	return (0);
}
