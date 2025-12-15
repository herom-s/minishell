/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 13:40:57 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/15 14:10:28 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHTABLE_H
# define HASHTABLE_H

# include "libft.h"
# include <stdlib.h>
# include <stdint.h>

# define INIT_CAPACITY 16
# define FNV_OFFSET 14695981039346656037UL
# define FNV_PRIME 1099511628211UL

typedef struct s_hashtable_item
{
	const char			*key;
	void				*value;
}						t_hashtable_item;

typedef struct s_hashtable
{
	t_list				**items;
	size_t				capacity;
	size_t				len;
}						t_hashtable;

typedef struct s_hashtable_iterator
{
	const char			*key;
	void				*value;

	t_hashtable			*table;
	size_t				index;
	t_list				*node;
}						t_hashtable_iterator;

t_hashtable				*hashtable_create(void);
void					hashtable_destroy(t_hashtable *table);

uint64_t				hashtable_key(const char *key);

t_hashtable_item		*hashtable_create_item(const char *key, void *value);
void					hashtable_destroy_item(void *item);
t_list					*hashtable_get_item(t_hashtable *table,
							const char *key, void *value);
void					*hashtable_get(t_hashtable *table, const char *key);
const char				*hashtable_set_item(t_hashtable *table,
							const char *key, void *value);
const char				*hashtable_set(t_hashtable *table,
							const char *key, void *value);

int						hashtable_delete(t_hashtable *table, const char *key);

int						hashtable_expand(t_hashtable *table);

t_hashtable_iterator	hashtable_iterator(t_hashtable *table);
int						hashtable_next(t_hashtable_iterator *it);

#endif
