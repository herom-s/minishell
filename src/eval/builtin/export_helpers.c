/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 13:00:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/05 13:00:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <stdlib.h>

static int	count_env_vars(t_list *order)
{
	int		count;
	t_list	*node;

	count = 0;
	node = order;
	while (node)
	{
		if (ft_strncmp((char *)node->content, "_", 2) != 0)
			count++;
		node = node->next;
	}
	return (count);
}

static char	**collect_keys(t_list *order, int count)
{
	char	**keys;
	t_list	*node;
	int		i;

	keys = ft_calloc(count + 1, sizeof(char *));
	if (!keys)
		return (NULL);
	node = order;
	i = 0;
	while (node)
	{
		if (ft_strncmp((char *)node->content, "_", 2) != 0)
			keys[i++] = (char *)node->content;
		node = node->next;
	}
	return (keys);
}

static void	sort_keys(char **keys, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(keys[j], keys[j + 1]) > 0)
			{
				tmp = keys[j];
				keys[j] = keys[j + 1];
				keys[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

char	*export_no_args(t_shell_env *env)
{
	char	*res;
	char	**keys;
	int		count;
	int		i;

	res = ft_strdup("");
	count = count_env_vars(env->order);
	if (count == 0)
		return (res);
	keys = collect_keys(env->order, count);
	if (!keys)
		return (res);
	sort_keys(keys, count);
	i = 0;
	while (i < count)
	{
		append_env_line(&res, keys[i], env);
		i++;
	}
	free(keys);
	return (res);
}
