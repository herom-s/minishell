/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:59:05 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 14:59:06 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "libft.h"
#include <dirent.h>

int	has_wildcard(const char *str)
{
	t_quote_state	state;
	int				i;

	if (!str)
		return (0);
	state = QUOTE_NONE;
	i = 0;
	while (str[i])
	{
		state = update_quote_state(str[i], state);
		if (str[i] == '*' && state == QUOTE_NONE)
			return (1);
		i++;
	}
	return (0);
}

int	match_pattern(const char *pattern, const char *str)
{
	if (!pattern || !str)
		return (0);
	while (*pattern && *str)
	{
		if (*pattern == '*')
		{
			pattern++;
			if (!*pattern)
				return (1);
			while (*str)
			{
				if (match_pattern(pattern, str))
					return (1);
				str++;
			}
			return (0);
		}
		if (*pattern != *str)
			return (0);
		pattern++;
		str++;
	}
	while (*pattern == '*')
		pattern++;
	return (!*pattern && !*str);
}

void	free_match_list(t_match_list *list)
{
	t_match_list	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		if (tmp->match)
			free(tmp->match);
		free(tmp);
	}
}

t_match_list	*add_match(t_match_list *list, const char *match)
{
	t_match_list	*new;
	t_match_list	*curr;

	new = ft_calloc(1, sizeof(t_match_list));
	if (!new)
		return (list);
	new->match = ft_strdup(match);
	new->next = NULL;
	if (!list)
		return (new);
	curr = list;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
	return (list);
}

int	count_matches(t_match_list *list)
{
	int	count;

	count = 0;
	while (list)
	{
		count++;
		list = list->next;
	}
	return (count);
}

void	sort_matches(char **matches, int count)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(matches[j], matches[j + 1]) > 0)
			{
				tmp = matches[j];
				matches[j] = matches[j + 1];
				matches[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

char	**match_list_to_array(t_match_list *list)
{
	char			**result;
	int				count;
	int				i;
	t_match_list	*curr;

	count = count_matches(list);
	if (count == 0)
		return (NULL);
	result = ft_calloc(count + 1, sizeof(char *));
	if (!result)
		return (NULL);
	curr = list;
	i = 0;
	while (curr)
	{
		result[i] = ft_strdup(curr->match);
		curr = curr->next;
		i++;
	}
	result[i] = NULL;
	sort_matches(result, count);
	return (result);
}
