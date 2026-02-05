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

t_match_list	*get_matching_files(const char *dir, const char *pattern)
{
	DIR				*dp;
	struct dirent	*entry;
	t_match_list	*matches;
	char			*full_path;

	if (!dir || !pattern)
		return (NULL);
	dp = opendir(dir);
	if (!dp)
		return (NULL);
	matches = NULL;
	entry = readdir(dp);
	while (entry)
	{
		if (should_include_file(entry->d_name, pattern)
			&& match_pattern(pattern, entry->d_name))
		{
			full_path = build_full_path(dir, entry->d_name);
			matches = add_match(matches, full_path);
			free(full_path);
		}
		entry = readdir(dp);
	}
	closedir(dp);
	return (matches);
}

char	**expand_wildcards(const char *pattern)
{
	char			*dir;
	char			*file_pattern;
	t_match_list	*matches;
	char			**result;

	if (!pattern || !has_wildcard(pattern))
		return (NULL);
	dir = extract_directory(pattern);
	file_pattern = extract_pattern(pattern);
	matches = get_matching_files(dir, file_pattern);
	free(dir);
	free(file_pattern);
	if (!matches)
		return (NULL);
	result = match_list_to_array(matches);
	free_match_list(matches);
	return (result);
}
