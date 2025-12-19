/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:44:09 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/05 16:54:36 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <stdlib.h>

t_cmd_response	*func_built_in_exit(t_ast *shell_ast, char **cmd_str,
		t_shell_env *env, char *envp[])
{
	t_cmd_response	*res;
	size_t			num_args;

	(void)shell_ast;
	(void)env;
	(void)envp;
	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
		return (NULL);
	num_args = num_arguments(cmd_str);
	if (num_args >= 1)
	{
		res->erro_msg = ft_strdup("too many arguments\n");
		res->exit_code = 1;
		return (res);
	}
	return (res);
}
