/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:39:22 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/15 16:52:32 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

static t_cmd_response	*pwd_error(t_cmd_response *res, char *curr_dir)
{
	ft_dprintf(STDERR_FILENO, "%s\n", strerror(errno));
	res->exit_code = 1;
	free(curr_dir);
	return (res);
}

t_cmd_response	*func_built_in_pwd(t_ast *shell_ast, char **cmd_str,
		t_shell_env *env)
{
	t_cmd_response	*res;
	char			*curr_dir;

	(void)shell_ast;
	(void)cmd_str;
	(void)env;
	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
		return (NULL);
	curr_dir = ft_calloc(1, PATH_MAX + 1);
	if (!curr_dir)
	{
		free(res);
		return (NULL);
	}
	if (!getcwd(curr_dir, PATH_MAX))
		return (pwd_error(res, curr_dir));
	ft_dprintf(STDOUT_FILENO, "%s\n", curr_dir);
	res->exit_code = 0;
	free(curr_dir);
	return (res);
}
