/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 16:27:16 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/29 17:09:13 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include <stdlib.h>
#include <signal.h>
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
		if (call->is_builtin)
			signal(SIGPIPE, SIG_IGN);
		if (res->output)
			ft_putstr_fd(res->output, STDOUT_FILENO);
		if (res->erro_msg)
			ft_putstr_fd(res->erro_msg, STDERR_FILENO);
		destroy_cmd_res(res);
	}
	return (code);
}

void	func_exec_cmd_pipe(t_ast *shell_ast, t_shell_env *env)
{
	int				exit_code;
	t_cmd_func_call	*call;

	exit_code = 1;
	if (shell_ast->type != AST_SIMPLE_CMD)
		child_exit(env, shell_ast, 1);
	call = check_cmd(shell_ast, env);
	if (!call)
		child_exit(env, shell_ast, 1);
	if (eval_redir(shell_ast) > 0)
		exit_code = execute_child_cmd_pipe(call, shell_ast);
	free_cmd_str(call->cmd_str);
	free(call);
	child_exit(env, shell_ast, exit_code);
}

static void	setup_pipe_fds(int dup_src, int dup_dest, int close_fd)
{
	if (dup2(dup_src, dup_dest) == -1)
		exit(EXIT_FAILURE);
	close(dup_src);
	close(close_fd);
}

void	run_pipe_child(t_ast *ast, t_shell_env *env, int *writer, int *reader)
{
	if (writer)
	{
		close(writer[INPUT_END]);
		setup_pipe_fds(writer[OUTPUT_END], STDOUT_FILENO, writer[OUTPUT_END]);
		eval_pipe_recursive(ast->u_ast.s_pipe_seq.left, env, writer);
	}
	else
	{
		close(reader[OUTPUT_END]);
		setup_pipe_fds(reader[INPUT_END], STDIN_FILENO, reader[INPUT_END]);
		eval_pipe_recursive(ast->u_ast.s_pipe_seq.right, env, reader);
	}
}

void	eval_pipe_recursive(t_ast *shell_ast, t_shell_env *env, int pipefd[2])
{
	pid_t	left_pid;
	int		new_fd[2];

	(void)pipefd;
	if (shell_ast->type == AST_SIMPLE_CMD)
		func_exec_cmd_pipe(shell_ast, env);
	if (shell_ast->type != AST_PIPE_SEQ || pipe(new_fd) == -1)
		child_exit(env, shell_ast, EXIT_FAILURE);
	left_pid = fork();
	if (left_pid == -1)
		child_exit(env, shell_ast, EXIT_FAILURE);
	if (left_pid == 0)
		run_pipe_child(shell_ast, env, new_fd, NULL);
	else
	{
		close(new_fd[OUTPUT_END]);
		run_pipe_child(shell_ast, env, NULL, new_fd);
	}
	child_exit(env, shell_ast, EXIT_FAILURE);
}
