/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:50:32 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/26 15:51:06 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include "minishell_signal.h"
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
		if (eval_redir(shell_ast) > 0)
			res->exit_code = 0;
	}
	restore_original_std_fds(env);
	return (res);
}

static t_cmd_response	*exec_cmd_with_call(t_ast *ast, t_cmd_func_call *call,
							t_shell_env *env)
{
	t_cmd_response	*res;

	(void)env;
	res = NULL;
	if (eval_redir(ast) > 0)
		res = exec_call(ast, call);
	else
	{
		res = create_cmd_res();
		if (res)
			res->exit_code = 1;
	}
	return (res);
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
	if (env)
		env->current_call = call;
	save_original_std_fds(env);
	res = exec_cmd_with_call(shell_ast, call, env);
	restore_original_std_fds(env);
	free_cmd_str(call->cmd_str);
	if (env)
		env->current_call = NULL;
	free(call);
	return (res);
}

static t_cmd_response	*eval_node(t_ast *node, t_shell_env *env)
{
	if (node->type == AST_SIMPLE_CMD)
		return (eval_cmd(node, env));
	else if (node->type == AST_PIPE_SEQ)
		return (eval_pipe(node, env));
	else if (node->type == AST_AND_OR)
		return (eval_and_or(node, env));
	else if (node->type == AST_SUBSHELL)
		return (eval_subshell(node, env));
	return (NULL);
}

t_shell_response	*eval_ast(t_ast *shell_ast, t_shell_env *env)
{
	t_shell_response	*res;
	t_cmd_response		*cmd_res;

	if (!shell_ast)
		return (NULL);
	if (process_heredocs(shell_ast, env) == -1)
	{
		cleanup_heredoc_files(shell_ast);
		return (NULL);
	}
	cmd_res = eval_node(shell_ast, env);
	cleanup_heredoc_files(shell_ast);
	if (!cmd_res)
		return (NULL);
	res = ft_calloc(1, sizeof(t_shell_response));
	if (!res)
		return (destroy_cmd_res(cmd_res));
	res->exit_code = cmd_res->exit_code;
	if (env)
		env->last_exit_code = res->exit_code;
	free(cmd_res);
	return (res);
}
