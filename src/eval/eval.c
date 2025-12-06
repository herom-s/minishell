/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:50:32 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/05 16:56:52 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

t_cmd_func_call	*check_cmd(t_ast *shell_ast, char *envp[])
{
	int					i;
	t_cmd_func_call		*call;
	const t_cmd_func	funcs[8] = {&func_built_in_cd, &func_built_in_pwd,
		&func_built_in_env, &func_built_in_export, &func_built_in_unset,
		&func_built_in_echo, &func_built_in_exit, &func_exec_cmd};

	i = 0;
	while (i < 8)
	{
		call = ft_calloc(1, sizeof(t_cmd_func_call));
		if (!call)
			return (NULL);
		call->cmd_func = funcs[i];
		call->cmd_str = get_cmd_str(shell_ast->s_simple_cmd.cmd_name,
				shell_ast->s_simple_cmd.cmd_suffix, envp);
		call->envp = envp;
		return (call);
		i++;
	}
	return (NULL);
}

t_shell_response	*eval_ast(t_ast *shell_ast, char *envp[])
{
	t_shell_response	*res;
	t_cmd_response		*cmd_res;
	t_cmd_func_call		*call;

	res = calloc(1, sizeof(t_shell_response));
	if (!res)
		return (NULL);
	call = check_cmd(shell_ast, envp);
	if (call)
	{
		cmd_res = call->cmd_func(shell_ast, call->cmd_str, envp);
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
