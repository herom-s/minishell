/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 14:40:09 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/08 15:00:22 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <unistd.h>

t_cmd_response	*func_exec_cmd(t_ast *shell_ast, char **cmd_str,
		t_shell_env *env, char *envp[])
{
	t_cmd_response	*res;

	(void)shell_ast;
	(void)env;
	if (execve(cmd_str[0], cmd_str, envp) < 0)
	{
		ft_dprintf(STDERR_FILENO, "%s: command not found\n", cmd_str[0]);
		res = ft_calloc(1, sizeof(t_cmd_response));
		if (res)
			res->exit_code = 127;
		return (res);
	}
	return (NULL);
}
