/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_str_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:52:26 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/03 11:55:16 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

char	**get_bin_paths(char *envp[])
{
	char	**default_paths;

	while (*envp && ft_strncmp(*envp, "PATH=", 5) != 0)
		envp++;
	if (!*envp)
	{
		default_paths = ft_calloc(6, sizeof(char *));
		if (!default_paths)
			return (NULL);
		default_paths[0] = ft_strdup("/usr/local/bin");
		default_paths[1] = ft_strdup("/usr/bin");
		default_paths[2] = ft_strdup("/bin");
		default_paths[3] = ft_strdup("/usr/sbin");
		default_paths[4] = ft_strdup("/sbin");
		default_paths[5] = NULL;
		return (default_paths);
	}
	return (ft_split(*envp + 5, ':'));
}

int	check_path(char *path, char *argv)
{
	char	*tmp_path;
	char	*full_path;
	int		result;

	tmp_path = ft_strjoin(path, "/");
	full_path = ft_strjoin(tmp_path, argv);
	free(tmp_path);
	result = access(full_path, X_OK);
	free(full_path);
	return (result == 0);
}
