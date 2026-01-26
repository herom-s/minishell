/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:50:32 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/26 15:51:06 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include "minishell_signal.h"
#include <fcntl.h>
#include <stdlib.h>
#include <readline/readline.h>
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
	setup_fork_signal(0);
	cmd_res = call->cmd_func(ast, call->cmd_str, call->env);
	if (cmd_res)
	{
		code = cmd_res->exit_code;
		destroy_cmd_res(cmd_res);
	}
	free_cmd_str(call->cmd_str);
	free(call);
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
	return (res);
}

t_cmd_response	*eval_cmd(t_ast *shell_ast, t_shell_env *env)
{
	t_cmd_response		*res;
	t_cmd_func_call		*call;

	res = NULL;
	call = check_cmd(shell_ast, env);
	if (!call)
		return (NULL);
	save_original_std_fds(env);
	if (eval_redir(shell_ast) > 0)
		res = exec_call(shell_ast, call);
	else
	{
		res = create_cmd_res();
		if (res)
			res->exit_code = 1;
	}
	restore_original_std_fds(env);
	free_cmd_str(call->cmd_str);
	free(call);
	return (res);
}

t_cmd_response	*eval_pipe(t_ast *shell_ast, t_shell_env *env)
{
	pid_t			root_pid;
	t_cmd_response	*res;

	if (shell_ast->type == AST_SIMPLE_CMD)
		return (eval_cmd(shell_ast, env));
	root_pid = fork();
	if (root_pid == -1)
		return (NULL);
	if (root_pid == 0)
	{
		setup_fork_signal(0);
		eval_pipe_recursive(shell_ast, env, NULL, -1);
		child_exit(env, shell_ast, 1);
	}
	setup_fork_signal(root_pid);
	res = handle_parent(root_pid);
	setup_nonfork_signal();
	return (res);
}

t_shell_response	*eval_ast(t_ast *shell_ast, t_shell_env *env)
{
	t_shell_response	*res;
	t_cmd_response		*cmd_res;

	if (env)
		env->root_node = shell_ast;
	if (process_heredocs(shell_ast, env) == -1)
	{
		cleanup_heredoc_files(shell_ast);
		return (NULL);
	}
	cmd_res = NULL;
	if (shell_ast->type == AST_SIMPLE_CMD)
		cmd_res = eval_cmd(shell_ast, env);
	else if (shell_ast->type == AST_PIPE_SEQ)
		cmd_res = eval_pipe(shell_ast, env);
	cleanup_heredoc_files(shell_ast);
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
