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

static void	cleanup_and_exit(t_ast *ast, t_shell_env *env,
				t_cmd_func_call *call, int code)
{
	free_cmd_str(call->cmd_str);
	if (env)
		env->current_call = NULL;
	free(call);
	child_exit(env, ast, code);
}

void	run_pipe_child(t_ast *ast, t_shell_env *env, t_pipe_context *ctx)
{
	if (ctx->writer)
	{
		close(ctx->writer[INPUT_END]);
		if (dup2(ctx->writer[OUTPUT_END], STDOUT_FILENO) == -1)
			child_exit(env, ast, EXIT_FAILURE);
		close(ctx->writer[OUTPUT_END]);
		eval_pipe_recursive(ast->u_ast.s_pipe_seq.left, env, ctx->writer,
			ctx->prev_pipe_fd_read_end);
	}
	else
	{
		close(ctx->reader[OUTPUT_END]);
		if (dup2(ctx->reader[INPUT_END], STDIN_FILENO) == -1)
			child_exit(env, ast, EXIT_FAILURE);
		close(ctx->reader[INPUT_END]);
		eval_pipe_recursive(ast->u_ast.s_pipe_seq.right, env, NULL,
			ctx->prev_pipe_fd_read_end);
	}
}

void	func_exec_cmd_pipe(t_ast *ast, t_shell_env *env, int *pipe_fd,
						int prev_pipe_read_fd)
{
	int				exit_code;
	t_cmd_func_call	*call;

	if (ast->type != AST_SIMPLE_CMD)
		child_exit(env, ast, 1);
	call = check_cmd(ast, env);
	if (!call)
		child_exit(env, ast, 1);
	if (!call->is_builtin)
		close_prev_and_pipe_fds(pipe_fd, prev_pipe_read_fd);
	if (eval_redir(ast, env) == -1)
		cleanup_and_exit(ast, env, call, 1);
	if (!call->cmd_func)
	{
		if (call->is_builtin)
			close_prev_and_pipe_fds(pipe_fd, prev_pipe_read_fd);
		cleanup_and_exit(ast, env, call, 0);
	}
	exit_code = execute_child_cmd_pipe(call, ast);
	if (call->is_builtin)
		close_prev_and_pipe_fds(pipe_fd, prev_pipe_read_fd);
	cleanup_and_exit(ast, env, call, exit_code);
}
