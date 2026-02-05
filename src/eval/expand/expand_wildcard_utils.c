/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:59:38 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 14:59:53 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "libft.h"
#include <dirent.h>

char	*extract_directory(const char *pattern)
{
	char	*last_slash;
	char	*dir;

	if (!pattern)
		return (ft_strdup("."));
	last_slash = ft_strrchr(pattern, '/');
	if (!last_slash)
		return (ft_strdup("."));
	dir = ft_substr(pattern, 0, last_slash - pattern + 1);
	return (dir);
}

char	*extract_pattern(const char *pattern)
{
	char	*last_slash;

	if (!pattern)
		return (ft_strdup("*"));
	last_slash = ft_strrchr(pattern, '/');
	if (!last_slash)
		return (ft_strdup(pattern));
	return (ft_strdup(last_slash + 1));
}

static int	should_include_file(const char *filename, const char *pattern)
{
	if (filename[0] == '.' && pattern[0] != '.')
		return (0);
	return (1);
}

static char	*build_full_path(const char *dir, const char *filename)
{
	char	*tmp;
	char	*result;

	if (ft_strcmp(dir, ".") == 0)
		return (ft_strdup(filename));
	tmp = ft_strjoin(dir, "/");
	result = ft_strjoin(tmp, filename);
	free(tmp);
	return (result);
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
