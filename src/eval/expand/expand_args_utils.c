/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 16:17:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/05 16:17:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "libft.h"

int	count_args(char **args)
{
	int	count;

	count = 0;
	if (!args)
		return (0);
	while (args[count])
		count++;
	return (count);
}

int	add_expanded_args(char **result, int pos, char **expanded)
{
	int	i;

	i = 0;
	while (expanded && expanded[i])
	{
		result[pos++] = ft_strdup(expanded[i]);
		i++;
	}
	return (pos);
}

void	free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	expand_and_add_wildcards(char **result, int pos, char *expanded,
	int do_wildcard)
{
	char	**wildcard_matches;

	if (do_wildcard && has_wildcard(expanded))
	{
		wildcard_matches = expand_wildcards(expanded);
		if (wildcard_matches)
		{
			pos = add_expanded_args(result, pos, wildcard_matches);
			free_str_array(wildcard_matches);
			free(expanded);
			return (pos);
		}
	}
	result[pos++] = expanded;
	return (pos);
}

int	should_skip_empty(const char *original, const char *expanded)
{
	if (!expanded || expanded[0] == '\0')
	{
		if (original && original[0] == '$')
			return (1);
	}
	return (0);
}
