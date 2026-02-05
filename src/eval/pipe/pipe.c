/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 13:45:38 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/26 16:29:34 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "minishell_signal.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

static void	handle_child_process(t_ast *ast, t_shell_env *env, int *pipe_fd,
		int *new_fd)
{
	t_pipe_context	ctx;

	if (env)
		env->interactive_owner = 0;
	if (pipe_fd)
		close(pipe_fd[INPUT_END]);
	close(new_fd[INPUT_END]);
	ctx = (t_pipe_context){new_fd, NULL, -1, 0};
	run_pipe_child(ast, env, &ctx);
}

static void	wait_and_exit(t_ast *ast, t_shell_env *env, pid_t left, pid_t right)
{
	int	status;

	waitpid(left, &status, 0);
	waitpid(right, &status, 0);
	if (WIFEXITED(status))
		child_exit(env, ast, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		child_exit(env, ast, 128 + WTERMSIG(status));
	child_exit(env, ast, EXIT_FAILURE);
}

static void	handle_parent_process(t_ast *ast, t_shell_env *env,
		t_pipe_context *ctx)
{
	t_pipe_context	child_ctx;
	int				status;
	pid_t			right_pid;

	close(ctx->writer[OUTPUT_END]);
	right_pid = fork();
	if (right_pid == -1)
	{
		waitpid(ctx->left_pid, &status, 0);
		child_exit(env, ast, EXIT_FAILURE);
	}
	if (right_pid == 0)
	{
		child_ctx = (t_pipe_context){NULL, ctx->writer,
			ctx->prev_pipe_fd_read_end, 0};
		run_pipe_child(ast, env, &child_ctx);
		child_exit(env, ast, EXIT_FAILURE);
	}
	close(ctx->writer[INPUT_END]);
	setup_fork_signal(right_pid);
	wait_and_exit(ast, env, ctx->left_pid, right_pid);
}

static void	init_parent_ctx(t_pipe_context *ctx, int *new_fd, int *pipe_fd,
							pid_t left_pid)
{
	ctx->writer = new_fd;
	ctx->reader = NULL;
	ctx->prev_pipe_fd_read_end = -1;
	if (pipe_fd)
		ctx->prev_pipe_fd_read_end = pipe_fd[INPUT_END];
	ctx->left_pid = left_pid;
}

void	eval_pipe_recursive(t_ast *ast, t_shell_env *env, int *pipe_fd,
		int prev_pipe_read_fd)
{
	t_pipe_context	ctx;
	pid_t			left_pid;
	int				new_fd[2];

	(void)prev_pipe_read_fd;
	if (ast->type == AST_SIMPLE_CMD)
		func_exec_cmd_pipe(ast, env, pipe_fd, prev_pipe_read_fd);
	if (ast->type == AST_SUBSHELL)
	{
		if (eval_io_file(ast->u_ast.s_subshell.io_file, env) == -1)
			child_exit(env, ast, 1);
		eval_subshell_in_pipe(ast, env);
	}
	if (ast->type != AST_PIPE_SEQ || pipe(new_fd) == -1)
		child_exit(env, ast, EXIT_FAILURE);
	left_pid = fork();
	if (left_pid == -1)
		child_exit(env, ast, EXIT_FAILURE);
	if (left_pid == 0)
	{
		handle_child_process(ast, env, pipe_fd, new_fd);
		child_exit(env, ast, EXIT_FAILURE);
	}
	init_parent_ctx(&ctx, new_fd, pipe_fd, left_pid);
	handle_parent_process(ast, env, &ctx);
}
	