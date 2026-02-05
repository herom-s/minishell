/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 12:00:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 12:00:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "minishell_signal.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	exec_child_cmd(t_ast *ast, t_cmd_func_call *call)
{
	t_cmd_response	*cmd_res;
	t_shell_env		*env;
	int				code;

	code = 1;
	env = call->env;
	setup_fork_signal(0);
	if (env)
	{
		env->interactive_owner = 0;
		env->current_call = call;
	}
	cmd_res = call->cmd_func(ast, call->cmd_str, call->env);
	if (cmd_res)
	{
		code = cmd_res->exit_code;
		destroy_cmd_res(cmd_res);
	}
	free_cmd_str(call->cmd_str);
	free(call);
	if (env)
		env->current_call = NULL;
	child_exit(env, ast, code);
}

t_cmd_response	*eval_external_cmd(t_ast *shell_ast, t_cmd_func_call *call)
{
	pid_t			cmd_pid;
	t_cmd_response	*res;

	cmd_pid = fork();
	if (cmd_pid == -1)
		return (NULL);
	if (cmd_pid == 0)
		exec_child_cmd(shell_ast, call);
	setup_fork_signal(cmd_pid);
	res = handle_parent(cmd_pid);
	setup_nonfork_signal();
	restore_terminal_settings(&call->env->saved_termios,
		call->env->termios_saved);
	return (res);
}
