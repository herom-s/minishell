/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 12:00:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/05 12:00:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "minishell_signal.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void	subshell_child(t_ast *shell_ast, t_shell_env *env)
{
	t_shell_response	*sub_res;
	int					code;

	setup_fork_signal(0);
	if (env)
		env->interactive_owner = 0;
	if (eval_io_file(shell_ast->u_ast.s_subshell.io_file, env) == -1)
		child_exit(env, shell_ast, 1);
	sub_res = eval_ast(shell_ast->u_ast.s_subshell.and_or, env);
	code = 0;
	if (sub_res)
		code = sub_res->exit_code;
	if (sub_res)
		free(sub_res);
	child_exit(env, shell_ast, code);
}

t_cmd_response	*eval_subshell(t_ast *shell_ast, t_shell_env *env)
{
	pid_t			pid;
	int				status;
	t_cmd_response	*cmd_res;

	if (!shell_ast)
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (NULL);
	if (pid == 0)
		subshell_child(shell_ast, env);
	setup_fork_signal(pid);
	waitpid(pid, &status, 0);
	setup_nonfork_signal();
	cmd_res = create_cmd_res();
	if (!cmd_res)
		return (NULL);
	if (WIFEXITED(status))
		cmd_res->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		cmd_res->exit_code = 128 + WTERMSIG(status);
	return (cmd_res);
}

void	eval_subshell_in_pipe(t_ast *shell_ast, t_shell_env *env)
{
	t_shell_response	*sub_res;
	int					code;

	sub_res = eval_ast(shell_ast->u_ast.s_subshell.and_or, env);
	code = 0;
	if (sub_res)
		code = sub_res->exit_code;
	if (sub_res)
		free(sub_res);
	child_exit(env, shell_ast, code);
}
