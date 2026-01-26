/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 13:45:38 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/08 16:19:32 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

static void	setup_pipe_fds(int dup_src, int dup_dest, int close_fd)
{
	if (dup2(dup_src, dup_dest) == -1)
		exit(EXIT_FAILURE);
	close(dup_src);
	close(close_fd);
}

void	run_pipe_child(t_ast *ast, t_shell_env *env, t_pipe_context *ctx)
{
	if (ctx->writer)
	{
		setup_pipe_fds(ctx->writer[OUTPUT_END], STDOUT_FILENO,
			ctx->writer[OUTPUT_END]);
		eval_pipe_recursive(ast->u_ast.s_pipe_seq.left, env, ctx->writer,
			ctx->prev_pipe_fd_read_end);
	}
	else
	{
		close(ctx->reader[OUTPUT_END]);
		setup_pipe_fds(ctx->reader[INPUT_END], STDIN_FILENO,
			ctx->reader[INPUT_END]);
		eval_pipe_recursive(ast->u_ast.s_pipe_seq.right, env, NULL,
			ctx->prev_pipe_fd_read_end);
	}
}

static void	handle_child_process(t_ast *ast, t_shell_env *env,
				int *pipe_fd, int *new_fd)
{
	t_pipe_context	ctx;

	if (pipe_fd)
		close(pipe_fd[INPUT_END]);
	close(new_fd[INPUT_END]);
	ctx = (t_pipe_context){new_fd, NULL, -1};
	run_pipe_child(ast, env, &ctx);
}

static void	handle_parent_process(t_ast *ast, t_shell_env *env,
				int *pipe_fd, int *new_fd)
{
	t_pipe_context	ctx;
	int				prev_pipe_read_fd;

	close(new_fd[OUTPUT_END]);
	prev_pipe_read_fd = -1;
	if (pipe_fd)
		prev_pipe_read_fd = pipe_fd[INPUT_END];
	ctx = (t_pipe_context){NULL, new_fd, prev_pipe_read_fd};
	run_pipe_child(ast, env, &ctx);
}

void	eval_pipe_recursive(t_ast *ast, t_shell_env *env, int *pipe_fd,
						int prev_pipe_read_fd)
{
	pid_t			left_pid;
	int				new_fd[2];

	if (ast->type == AST_SIMPLE_CMD)
		func_exec_cmd_pipe(ast, env, pipe_fd, prev_pipe_read_fd);
	if (ast->type != AST_PIPE_SEQ || pipe(new_fd) == -1)
		child_exit(env, ast, EXIT_FAILURE);
	left_pid = fork();
	if (left_pid == -1)
		child_exit(env, ast, EXIT_FAILURE);
	if (left_pid == 0)
		handle_child_process(ast, env, pipe_fd, new_fd);
	else
		handle_parent_process(ast, env, pipe_fd, new_fd);
	child_exit(env, ast, EXIT_FAILURE);
}
