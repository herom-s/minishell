/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:50:32 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/09 21:02:57 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

t_cmd_func_call	*check_cmd(t_ast *shell_ast, t_shell_env *env,
		char *envp[])
{
	t_cmd_func_call	*call;

	call = ft_calloc(1, sizeof(t_cmd_func_call));
	if (!call)
		return (NULL);
	call->env = env;
	call->cmd_str = get_cmd_str(shell_ast->s_simple_cmd.cmd_name,
			shell_ast->s_simple_cmd.cmd_suffix, envp);
	call->envp = envp;
	call->cmd_func = get_cmd_func(shell_ast->s_simple_cmd.cmd_name);
	return (call);
}

t_shell_response	*eval_ast(t_ast *shell_ast, t_shell_env *env, char *envp[])
{
	t_shell_response	*res;
	t_cmd_response		*cmd_res;
	t_cmd_func_call		*call;

	res = ft_calloc(1, sizeof(t_shell_response));
	if (!res)
		return (NULL);
	call = check_cmd(shell_ast, env, envp);
	if (call)
	{
		cmd_res = call->cmd_func(shell_ast, call->cmd_str, call->env,
				envp);
		if (cmd_res)
		{
			res->output = cmd_res->output;
			res->exit_code = cmd_res->exit_code;
			res->erro_msg = cmd_res->erro_msg;
			res->curr_dir = cmd_res->curr_dir;
			free(cmd_res);
		}
		free_cmd_str(call->cmd_str);
		free(call);
	}
	return (res);
}
