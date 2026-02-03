/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:41:01 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/29 17:02:11 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void	update_env_var(t_shell_env *env, const char *key, char *value)
{
	char	*old_value;

	old_value = hashtable_get(env->vars, key);
	if (old_value)
		free(old_value);
	hashtable_set(env->vars, key, value);
}

static char	*get_target_dir(char **cmd_str, t_shell_env *env, int *print_pwd)
{
	char	*path;

	if (!cmd_str[1])
	{
		path = hashtable_get(env->vars, "HOME");
		if (!path)
			ft_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
	}
	else if (ft_strncmp(cmd_str[1], "-", 2) == 0)
	{
		path = hashtable_get(env->vars, "OLDPWD");
		if (!path)
			ft_dprintf(STDERR_FILENO, "minishell: cd: OLDPWD not set\n");
		else
			*print_pwd = 1;
	}
	else
		path = cmd_str[1];
	return (path);
}

static int	change_dir_and_update(t_shell_env *env, char *path, char *old_pwd)
{
	char	*new_pwd;

	if (chdir(path) == -1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", path,
			strerror(errno));
		free(old_pwd);
		return (1);
	}
	update_env_var(env, "OLDPWD", old_pwd);
	new_pwd = ft_calloc(1, PATH_MAX + 1);
	if (!new_pwd)
		return (1);
	if (getcwd(new_pwd, PATH_MAX))
		update_env_var(env, "PWD", new_pwd);
	else
		free(new_pwd);
	return (0);
}

static t_cmd_response	*init_cd_response(char **cmd_str)
{
	t_cmd_response	*res;

	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
		return (NULL);
	if (num_arguments(cmd_str) > 1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: too many arguments\n");
		res->exit_code = 1;
	}
	return (res);
}

t_cmd_response	*func_built_in_cd(t_ast *shell_ast, char **cmd_str,
		t_shell_env *env)
{
	t_cmd_response	*res;
	char			*path;
	char			*old_pwd;
	int				print_pwd;

	(void)shell_ast;
	res = init_cd_response(cmd_str);
	if (!res || res->exit_code)
		return (res);
	print_pwd = 0;
	path = get_target_dir(cmd_str, env, &print_pwd);
	if (!path)
	{
		res->exit_code = 1;
		return (res);
	}
	old_pwd = ft_calloc(1, PATH_MAX + 1);
	if (old_pwd)
		getcwd(old_pwd, PATH_MAX);
	res->exit_code = change_dir_and_update(env, path, old_pwd);
	if (res->exit_code == 0 && print_pwd)
		ft_dprintf(STDOUT_FILENO, "%s\n", hashtable_get(env->vars, "PWD"));
	return (res);
}
