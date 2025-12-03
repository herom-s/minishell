/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:39:22 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/03 11:39:48 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

char	*add_newline(char *curr_dir)
{
	char	*tmp;

	tmp = ft_strjoin(curr_dir, "\n");
	free(curr_dir);
	curr_dir = tmp;
	return (curr_dir);
}

t_cmd_response	*func_built_in_pwd(t_ast *shell_ast, char **cmd_str,
		char *envp[])
{
	t_cmd_response	*res;
	char			*curr_dir;

	(void)shell_ast;
	(void)cmd_str;
	(void)envp;
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
	{
		res->erro_msg = ft_strdup(strerror(errno));
		res->exit_code = 1;
		free(curr_dir);
		return (res);
	}
	res->output = add_newline(curr_dir);
	res->exit_code = 0;
	return (res);
}
