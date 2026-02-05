/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:50:32 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 15:01:32 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static t_cmd_response	*handle_no_cmd_name(t_ast *shell_ast, t_shell_env *env)
{
	t_cmd_response	*res;

	save_original_std_fds(env);
	res = create_cmd_res();
	if (res)
	{
		res->exit_code = 1;
		if (eval_redir(shell_ast, env) > 0)
			res->exit_code = 0;
	}
	restore_original_std_fds(env);
	return (res);
}

static void	cleanup_call(t_cmd_func_call *call, t_shell_env *env)
{
	free_cmd_str(call->cmd_str);
	if (env)
		env->current_call = NULL;
	free(call);
}

static t_cmd_response	*exec_cmd_with_call(t_ast *ast, t_cmd_func_call *call,
							t_shell_env *env)
{
	t_cmd_response	*res;

	res = NULL;
	if (eval_redir(ast, env) > 0)
		res = exec_call(ast, call);
	else
	{
		res = create_cmd_res();
		if (res)
			res->exit_code = 1;
	}
	return (res);
}

static int	is_empty_cmd(t_cmd_func_call *call)
{
	if (!call->cmd_str || !call->cmd_str[0])
		return (1);
	if (call->cmd_str[0][0] == '\0')
		return (1);
	return (0);
}

t_cmd_response	*eval_cmd(t_ast *shell_ast, t_shell_env *env)
{
	t_cmd_response	*res;
	t_cmd_func_call	*call;

	if (!shell_ast->u_ast.s_simple_cmd.cmd_name)
		return (handle_no_cmd_name(shell_ast, env));
	call = check_cmd(shell_ast, env);
	if (!call)
		return (NULL);
	if (is_empty_cmd(call))
	{
		res = create_cmd_res();
		if (res)
			res->exit_code = 0;
		cleanup_call(call, env);
		return (res);
	}
	if (env)
		env->current_call = call;
	save_original_std_fds(env);
	res = exec_cmd_with_call(shell_ast, call, env);
	restore_original_std_fds(env);
	cleanup_call(call, env);
	return (res);
}
