/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 14:40:57 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/05 16:54:23 by hermarti         ###   ########.fr       */
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

size_t	num_arguments(char **cmd_str)
{
	size_t	i;

	i = 0;
	while (cmd_str[i + 1])
		i++;
	return (i);
}
