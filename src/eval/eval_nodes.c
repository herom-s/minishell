/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 12:00:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/03 12:00:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include "minishell_signal.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

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
		if (env)
			env->interactive_owner = 0;
		eval_pipe_recursive(shell_ast, env, NULL, -1);
		child_exit(env, shell_ast, 1);
	}
	setup_fork_signal(root_pid);
	res = handle_parent(root_pid);
	setup_nonfork_signal();
	return (res);
}

t_cmd_response	*eval_node(t_ast *node, t_shell_env *env)
{
	if (node->type == AST_SIMPLE_CMD)
		return (eval_cmd(node, env));
	else if (node->type == AST_PIPE_SEQ)
		return (eval_pipe(node, env));
	else if (node->type == AST_AND_OR)
		return (eval_and_or(node, env));
	else if (node->type == AST_SUBSHELL)
		return (eval_subshell(node, env));
	return (NULL);
}

t_cmd_response	*eval_and_or(t_ast *shell_ast, t_shell_env *env)
{
	t_cmd_response	*left_res;
	t_cmd_response	*right_res;
	t_ast			*left;
	t_ast			*right;
	t_token_type	op_type;

	if (!shell_ast)
		return (NULL);
	left = shell_ast->u_ast.s_and_or.left;
	right = shell_ast->u_ast.s_and_or.right;
	left_res = eval_node(left, env);
	if (!left_res)
		return (NULL);
	op_type = shell_ast->u_ast.s_and_or.op->type;
	if ((op_type == AND_IF && left_res->exit_code == 0)
		|| (op_type == OR_IF && left_res->exit_code != 0))
	{
		destroy_cmd_res(left_res);
		right_res = eval_node(right, env);
		if (!right_res)
			return (NULL);
		return (right_res);
	}
	return (left_res);
}

t_shell_response	*eval_ast(t_ast *shell_ast, t_shell_env *env)
{
	t_shell_response	*res;
	t_cmd_response		*cmd_res;

	if (!shell_ast)
		return (NULL);
	if (process_heredocs(shell_ast, env) == -1)
	{
		cleanup_heredoc_files(shell_ast);
		if (env)
			env->last_exit_code = 130;
		return (NULL);
	}
	cmd_res = eval_node(shell_ast, env);
	cleanup_heredoc_files(shell_ast);
	if (!cmd_res)
		return (NULL);
	res = ft_calloc(1, sizeof(t_shell_response));
	if (!res)
		return (destroy_cmd_res(cmd_res));
	res->exit_code = cmd_res->exit_code;
	if (env)
		env->last_exit_code = res->exit_code;
	free(cmd_res);
	return (res);
}
