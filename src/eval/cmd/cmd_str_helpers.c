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
#include "ast.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>

int	count_cmd_suffix(t_ast *cmd_suffix)
{
	int		count;
	t_ast	*node;

	count = 0;
	node = cmd_suffix;
	while (node)
	{
		if (node->u_ast.s_cmd_suffix.word)
			count++;
		node = node->u_ast.s_cmd_suffix.cmd_suffix;
	}
	return (count);
}

void	free_cmd_str(char **cmd_str)
{
	int	i;

	if (!cmd_str)
		return ;
	i = 0;
	while (cmd_str[i])
	{
		free(cmd_str[i]);
		i++;
	}
	free(cmd_str);
}

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

char	*resolve_command_path(char *cmd_name, t_shell_env *env)
{
	char	**bin_paths;
	int		i;
	char	*found_path;

	if (ft_strchr(cmd_name, '/') || check_builtin(cmd_name))
		return (ft_strdup(""));
	bin_paths = get_bin_paths(env);
	i = 0;
	while (bin_paths[i] && !check_path(bin_paths[i], cmd_name))
		i++;
	if (bin_paths[i])
		found_path = ft_strdup(bin_paths[i]);
	else
		found_path = ft_strdup("");
	free_cmd_str(bin_paths);
	return (found_path);
}
