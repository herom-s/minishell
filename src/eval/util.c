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
#include "eval.h"
#include <limits.h>
#include <unistd.h>

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

t_cmd_func	get_cmd_func(char *cmd_name)
{
	const char			*names[] = {"cd", "pwd", "env", "export", "unset",
		"echo", "exit", NULL};
	const t_cmd_func	funcs[] = {&func_built_in_cd, &func_built_in_pwd,
		&func_built_in_env, &func_built_in_export, &func_built_in_unset,
		&func_built_in_echo, &func_built_in_exit};
	int					i;

	i = 0;
	while (names[i])
	{
		if (ft_strcmp(cmd_name, names[i]) == 0)
			return (funcs[i]);
		i++;
	}
	return (&func_exec_cmd);
}
