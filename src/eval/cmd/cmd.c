/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 14:40:09 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 15:00:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

static int	is_directory(char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (1);
	}
	return (0);
}

static int	get_exec_error(char *cmd_path)
{
	if (ft_strchr(cmd_path, '/') && access(cmd_path, F_OK) == 0)
	{
		if (is_directory(cmd_path))
		{
			ft_dprintf(STDERR_FILENO, "%s: Is a directory\n", cmd_path);
			return (126);
		}
		if (access(cmd_path, X_OK) != 0)
		{
			ft_dprintf(STDERR_FILENO, "%s: Permission denied\n", cmd_path);
			return (126);
		}
	}
	if (ft_strchr(cmd_path, '/'))
	{
		ft_dprintf(STDERR_FILENO, "%s: No such file or directory\n", cmd_path);
		return (127);
	}
	ft_dprintf(STDERR_FILENO, "%s: command not found\n", cmd_path);
	return (127);
}

t_cmd_response	*func_exec_cmd(t_ast *shell_ast, char **cmd_str,
		t_shell_env *env)
{
	t_cmd_response	*res;

	(void)shell_ast;
	if (env->og_stdin_fd >= 0)
		close(env->og_stdin_fd);
	if (env->og_stdout_fd >= 0)
		close(env->og_stdout_fd);
	if (execve(cmd_str[0], cmd_str, env->envp) < 0)
	{
		res = ft_calloc(1, sizeof(t_cmd_response));
		if (res)
			res->exit_code = get_exec_error(cmd_str[0]);
		return (res);
	}
	return (NULL);
}
