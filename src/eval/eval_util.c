/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 16:17:45 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 15:30:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "expand.h"
#include "libft.h"
#include <unistd.h>

t_cmd_func_call	*check_cmd(t_ast *shell_ast, t_shell_env *env)
{
	t_cmd_func_call	*call;
	char			*cmd_name;

	call = ft_calloc(1, sizeof(t_cmd_func_call));
	if (!call)
		return (NULL);
	call->env = env;
	if (env)
		env->current_call = call;
	call->envp = env->envp;
	call->cmd_str = get_cmd_str(shell_ast->u_ast.s_simple_cmd.cmd_name,
			shell_ast->u_ast.s_simple_cmd.cmd_suffix, env);
	if (call->cmd_str && call->cmd_str[0] && call->cmd_str[0][0] != '\0')
	{
		cmd_name = call->cmd_str[0];
		if (ft_strchr(cmd_name, '/'))
			cmd_name = ft_strrchr(cmd_name, '/') + 1;
		call->cmd_func = get_cmd_func(cmd_name);
		call->is_builtin = check_builtin(cmd_name);
	}
	else
	{
		call->cmd_func = NULL;
		call->is_builtin = 0;
	}
	return (call);
}

t_cmd_response	*exec_call(t_ast *ast, t_cmd_func_call *call)
{
	if (call->is_builtin)
		return (call->cmd_func(ast, call->cmd_str, call->env));
	return (eval_external_cmd(ast, call));
}
