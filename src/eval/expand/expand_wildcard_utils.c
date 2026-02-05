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

int	should_include_file(const char *filename, const char *pattern)
{
	if (filename[0] == '.' && pattern[0] != '.')
		return (0);
	return (1);
}

char	*build_full_path(const char *dir, const char *filename)
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
