/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 16:01:12 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/26 16:13:59 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include <stdlib.h>
#include <unistd.h>

static int	execute_child_cmd_pipe(t_cmd_func_call *call, t_ast *ast)
{
	t_cmd_response	*res;
	int				code;

	code = 1;
	res = call->cmd_func(ast, call->cmd_str, call->env);
	if (res)
	{
		code = res->exit_code;
		destroy_cmd_res(res);
	}
	return (code);
}

static void	close_prev_and_pipe_fds(int pipe_fd[2], int prev_pipe_read_fd)
{
	if (pipe_fd)
		close(pipe_fd[INPUT_END]);
	if (prev_pipe_read_fd != -1)
		close(prev_pipe_read_fd);
}

void	func_exec_cmd_pipe(t_ast *ast, t_shell_env *env, int *pipe_fd,
						int prev_pipe_read_fd)
{
	int				exit_code;
	t_cmd_func_call	*call;

	exit_code = 1;
	if (ast->type != AST_SIMPLE_CMD)
		child_exit(env, ast, 1);
	call = check_cmd(ast, env);
	if (!call)
		child_exit(env, ast, 1);
	if (!call->is_builtin)
		close_prev_and_pipe_fds(pipe_fd, prev_pipe_read_fd);
	if (eval_redir(ast) == -1)
	{
		free_cmd_str(call->cmd_str);
		free(call);
		child_exit(env, ast, 1);
	}
	exit_code = execute_child_cmd_pipe(call, ast);
	if (call->is_builtin)
		close_prev_and_pipe_fds(pipe_fd, prev_pipe_read_fd);
	free_cmd_str(call->cmd_str);
	free(call);
	child_exit(env, ast, exit_code);
}
