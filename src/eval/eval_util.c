/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 16:17:45 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/26 15:49:31 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "eval.h"
#include <unistd.h>

t_cmd_func_call	*check_cmd(t_ast *shell_ast, t_shell_env *env)
{
	t_cmd_func_call	*call;

	call = ft_calloc(1, sizeof(t_cmd_func_call));
	if (!call)
		return (NULL);
	call->env = env;
	call->envp = env->envp;
	call->cmd_str = get_cmd_str(shell_ast->u_ast.s_simple_cmd.cmd_name,
			shell_ast->u_ast.s_simple_cmd.cmd_suffix, env->envp);
	call->cmd_func = get_cmd_func(shell_ast->u_ast.s_simple_cmd.cmd_name);
	call->is_builtin = check_builtin(shell_ast->u_ast.s_simple_cmd.cmd_name);
	return (call);
}

t_cmd_response	*exec_call(t_ast *ast, t_cmd_func_call *call)
{
	if (call->is_builtin)
		return (call->cmd_func(ast, call->cmd_str, call->env));
	return (eval_external_cmd(ast, call));
}
