/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:44:09 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/29 17:02:40 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

t_cmd_response	*func_built_in_exit(t_ast *shell_ast, char **cmd_str,
		t_shell_env *env)
{
	t_cmd_response	*res;
	size_t			num_args;

	(void)shell_ast;
	(void)env;
	res = ft_calloc(1, sizeof(t_cmd_response));
	if (!res)
		return (NULL);
	num_args = num_arguments(cmd_str);
	if (num_args >= 1)
	{
		ft_dprintf(STDERR_FILENO, "too many arguments\n");
		res->exit_code = 1;
		return (res);
	}
	ft_dprintf(STDERR_FILENO, "exit\n");
	res->exit_code = 0;
	return (res);
}
