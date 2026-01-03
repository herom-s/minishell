/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:50:32 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/29 17:12:38 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void	exec_child_cmd(t_ast *ast, t_cmd_func_call *call, int fds[2][2])
{
	t_cmd_response	*cmd_res;
	t_shell_env		*env;
	int				code;

	call = ft_calloc(1, sizeof(t_cmd_func_call));
	if (!call)
		return (NULL);
	call->env = env;
	call->cmd_str = get_cmd_str(shell_ast->u_ast.s_simple_cmd.cmd_name,
			shell_ast->u_ast.s_simple_cmd.cmd_suffix, envp);
	call->envp = envp;
	call->cmd_func = get_cmd_func(shell_ast->u_ast.s_simple_cmd.cmd_name);
	return (call);
}

t_cmd_response	*eval_external_cmd(t_ast *shell_ast, t_cmd_func_call *call)
{
	int		fds[2][2];
	pid_t	cmd_pid;

	create_pipes_or_fail(fds[PIPE_OUT], fds[PIPE_ERR]);
	cmd_pid = fork();
	if (cmd_pid == -1)
		return (closes_pipes(fds[PIPE_OUT], fds[PIPE_ERR]));
	if (cmd_pid == 0)
		exec_child_cmd(shell_ast, call, fds);
	return (handle_parent(cmd_pid, fds[PIPE_OUT], fds[PIPE_ERR]));
}

t_cmd_response	*eval_cmd(t_ast *shell_ast, t_shell_env *env)
{
	t_cmd_response	*cmd_res;
	t_cmd_func_call	*call;

	call = check_cmd(shell_ast, env);
	if (!call)
		return (NULL);
	if (call->is_builtin)
		cmd_res = call->cmd_func(shell_ast, call->cmd_str, call->env);
	else
		cmd_res = eval_external_cmd(shell_ast, call);
	free_cmd_str(call->cmd_str);
	free(call);
	return (cmd_res);
}

t_cmd_response	*eval_pipe(t_ast *shell_ast, t_shell_env *env)
{
	int		fds[2][2];
	pid_t	root_pid;

	if (shell_ast->type == AST_SIMPLE_CMD)
		return (eval_cmd(shell_ast, env));
	create_pipes_or_fail(fds[PIPE_OUT], fds[PIPE_ERR]);
	root_pid = fork();
	if (root_pid == -1)
		return (closes_pipes(fds[PIPE_OUT], fds[PIPE_ERR]));
	if (root_pid == 0)
	{
		create_child_fds(fds[PIPE_OUT], fds[PIPE_ERR]);
		eval_pipe_recursive(shell_ast, env, NULL);
		child_exit(env, shell_ast, 1);
	}
	return (handle_parent(root_pid, fds[PIPE_OUT], fds[PIPE_ERR]));
}

t_shell_response	*eval_ast(t_ast *shell_ast, t_shell_env *env)
{
	t_shell_response	*res;
	t_cmd_response		*cmd_res;

	if (env)
		env->root_node = shell_ast;
	cmd_res = NULL;
	if (shell_ast->type == AST_SIMPLE_CMD)
		cmd_res = eval_cmd(shell_ast, env);
	else if (shell_ast->type == AST_PIPE_SEQ)
		cmd_res = eval_pipe(shell_ast, env);
	if (!cmd_res)
		return (NULL);
	res = ft_calloc(1, sizeof(t_shell_response));
	if (!res)
		return (destroy_cmd_res(cmd_res));
	res->output = cmd_res->output;
	res->exit_code = cmd_res->exit_code;
	res->erro_msg = cmd_res->erro_msg;
	res->curr_dir = cmd_res->curr_dir;
	free(cmd_res);
	return (res);
}
