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

#include "eval.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void	save_original_std_fds(t_shell_env *env)
{
	env->og_stdout_fd = dup(STDOUT_FILENO);
	env->og_stdin_fd = dup(STDIN_FILENO);
}

void	child_exit(t_shell_env *env, t_ast *local_ast, int code)
{
	if (env && env->root_node)
		free_ast(env->root_node);
	else if (local_ast)
		free_ast(local_ast);
	if (env)
	{
		close(env->og_stdout_fd);
		close(env->og_stdin_fd);
		destroy_shell_env(env);
	}
	exit(code);
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
		res->exit_code = WEXITSTATUS(status);
	else
		res->exit_code = 1;
	return (res);
}
