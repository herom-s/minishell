/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 14:40:57 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/04 15:13:50 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <limits.h>

char	*get_curdir(void)
{
	char	*curr_dir;

	curr_dir = ft_calloc(1, PATH_MAX + 1);
	if (!curr_dir)
	{
		return (NULL);
	}
	getcwd(curr_dir, PATH_MAX);
	return (curr_dir);
}
