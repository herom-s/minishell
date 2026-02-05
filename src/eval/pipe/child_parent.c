/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_parent.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 16:15:05 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/15 17:04:37 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "minishell_signal.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	save_original_std_fds(t_shell_env *env)
{
	env->og_stdout_fd = dup(STDOUT_FILENO);
	env->og_stdin_fd = dup(STDIN_FILENO);
}

void	close_saved_fds(t_shell_env *env)
{
	if (env->og_stdin_fd >= 0)
	{
		close(env->og_stdin_fd);
		env->og_stdin_fd = -1;
	}
	if (env->og_stdout_fd >= 0)
	{
		close(env->og_stdout_fd);
		env->og_stdout_fd = -1;
	}
}

void	restore_original_std_fds(t_shell_env *env)
{
	if (env->og_stdin_fd >= 0)
	{
		dup2(env->og_stdin_fd, STDIN_FILENO);
		close(env->og_stdin_fd);
		env->og_stdin_fd = -1;
	}
	if (env->og_stdout_fd >= 0)
	{
		dup2(env->og_stdout_fd, STDOUT_FILENO);
		close(env->og_stdout_fd);
		env->og_stdout_fd = -1;
	}
}

void	child_exit(t_shell_env *env, t_ast *local_ast, int code)
{
	if (env && env->root_node)
	{
		destroy_shell_env(env);
	}
	else
	{
		if (local_ast)
			free_ast(local_ast);
		if (env)
			destroy_shell_env(env);
	}
	exit((unsigned char)code);
}

t_cmd_response	*handle_parent(pid_t pid)
{
	t_cmd_response	*res;
	int				status;

	res = create_cmd_res();
	if (!res)
	{
		waitpid(pid, &status, 0);
		return (res);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		res->exit_code = WEXITSTATUS(status);
		if (res->exit_code > 128 && res->exit_code < 160)
			print_signal_msg(res->exit_code - 128);
	}
	else if (WIFSIGNALED(status))
	{
		print_signal_msg(WTERMSIG(status));
		res->exit_code = 128 + WTERMSIG(status);
	}
	return (res);
}
