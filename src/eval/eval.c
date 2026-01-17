/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:50:32 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/15 17:13:51 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include "minishell_signal.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	exec_child_cmd(t_ast *ast, t_cmd_func_call *call)
{
	t_cmd_response	*cmd_res;
	t_shell_env		*env;
	int				code;

	code = 1;
	env = call->env;
	save_original_std_fds(call->env);
	if (eval_redir(ast) > 0)
	{
		cmd_res = call->cmd_func(ast, call->cmd_str, call->env);
		if (cmd_res)
		{
			code = cmd_res->exit_code;
			destroy_cmd_res(cmd_res);
		}
	}
	free_cmd_str(call->cmd_str);
	free(call);
	child_exit(env, ast, code);
}

t_cmd_response	*eval_external_cmd(t_ast *shell_ast, t_cmd_func_call *call)
{
	pid_t	cmd_pid;

	cmd_pid = fork();
	if (cmd_pid == -1)
		return (NULL);
	if (cmd_pid == 0)
		exec_child_cmd(shell_ast, call);
	return (handle_parent(cmd_pid));
}

t_cmd_response	*eval_cmd(t_ast *shell_ast, t_shell_env *env)
{
	t_cmd_response	*cmd_res;
	t_cmd_func_call	*call;

	cmd_res = NULL;
	call = check_cmd(shell_ast, env);
	if (!call)
		return (NULL);
	if (has_heredoc(shell_ast))
		setup_heredoc_signal();
	if (call->is_builtin)
	{
		if (eval_redir(shell_ast) > 0)
			cmd_res = call->cmd_func(shell_ast, call->cmd_str, call->env);
		else
		{
			cmd_res = create_cmd_res();
			if (cmd_res)
				cmd_res->exit_code = 1;
		}
	}
	else
		cmd_res = eval_external_cmd(shell_ast, call);
	free_cmd_str(call->cmd_str);
	free(call);
	if (has_heredoc(shell_ast))
		setup_signal();
	return (cmd_res);
}

t_cmd_response	*eval_pipe(t_ast *shell_ast, t_shell_env *env)
{
	pid_t	root_pid;

	if (shell_ast->type == AST_SIMPLE_CMD)
		return (eval_cmd(shell_ast, env));
	root_pid = fork();
	if (root_pid == -1)
		return (NULL);
	if (root_pid == 0)
	{
		save_original_std_fds(env);
		eval_pipe_recursive(shell_ast, env, NULL, -1);
		child_exit(env, shell_ast, 1);
	}
	return (handle_parent(root_pid));
}

t_shell_response	*eval_ast(t_ast *shell_ast, t_shell_env *env)
{
	t_shell_response	*res;
	t_cmd_response		*cmd_res;

	if (env)
		env->root_node = shell_ast;
	cmd_res = NULL;
	if (shell_ast->type == AST_SIMPLE_CMD)
		cmd_res = eval_cmd(shell_ast, env);
	else if (shell_ast->type == AST_PIPE_SEQ)
		cmd_res = eval_pipe(shell_ast, env);
	if (!cmd_res)
		return (NULL);
	res = ft_calloc(1, sizeof(t_shell_response));
	if (!res)
		return (destroy_cmd_res(cmd_res));
	res->exit_code = cmd_res->exit_code;
	res->curr_dir = cmd_res->curr_dir;
	free(cmd_res);
	return (res);
}
