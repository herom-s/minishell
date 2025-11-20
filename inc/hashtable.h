/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 13:40:57 by hermarti          #+#    #+#             */
/*   Updated: 2025/11/20 14:54:39 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHTABLE_H
# define HASHTABLE_H

# include <stdlib.h>

typedef struct s_hashtable_item
{
	const char			*key;
	void				*value;
}						t_hashtable_item;

typedef struct s_hashtable
{
	t_hashtable_item	*items;
	size_t				capacity;
	size_t				len;
}						t_hashtable;

#endif
