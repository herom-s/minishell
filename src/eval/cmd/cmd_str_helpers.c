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

#include "eval.h"
#include "libft.h"
#include <unistd.h>

char	**get_bin_paths(t_shell_env *env)
{
	char	*path_value;
	char	**empty_paths;

	path_value = hashtable_get(env->vars, "PATH");
	if (!path_value)
	{
		empty_paths = ft_calloc(1, sizeof(char *));
		return (empty_paths);
	}
	return (ft_split(path_value, ':'));
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
