/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 16:15:05 by hermarti          #+#    #+#             */
/*   Updated: 2025/12/29 16:47:32 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void	create_child_fds(int *fd_out, int *fd_err)
{
	close(fd_out[INPUT_END]);
	close(fd_err[INPUT_END]);
	dup2(fd_out[OUTPUT_END], STDOUT_FILENO);
	dup2(fd_err[OUTPUT_END], STDERR_FILENO);
	close(fd_out[OUTPUT_END]);
	close(fd_err[OUTPUT_END]);
}

void	child_exit(t_shell_env *env, t_ast *local_ast, int code)
{
	if (env && env->root_node)
		free_ast(env->root_node);
	else if (local_ast)
		free_ast(local_ast);
	if (env)
		destroy_shell_env(env);
	exit(code);
}

t_cmd_response	*handle_parent(pid_t pid, int *fd_out, int *fd_err)
{
	t_cmd_response	*res;
	int				status;

	close(fd_out[OUTPUT_END]);
	close(fd_err[OUTPUT_END]);
	res = create_cmd_res();
	if (!res || !res->output || !res->erro_msg)
	{
		waitpid(pid, &status, 0);
		return (res);
	}
	read_stream(fd_out[INPUT_END], &res->output);
	read_stream(fd_err[INPUT_END], &res->erro_msg);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		res->exit_code = WEXITSTATUS(status);
	else
		res->exit_code = 1;
	return (res);
}
