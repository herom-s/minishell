/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:54:09 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/26 15:41:08 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int	handle_heredoc_io(t_ast *io, t_shell_env *env);

static int	process_simple_cmd_heredocs(t_ast *node, t_shell_env *env)
{
	t_ast	*it;

	it = node->u_ast.s_simple_cmd.cmd_prefix;
	while (it)
	{
		if (it->type == AST_CMD_PREFIX && it->u_ast.s_cmd_prefix.io_file
			&& it->u_ast.s_cmd_prefix.io_file->type == AST_IO_FILE)
			if (handle_heredoc_io(it->u_ast.s_cmd_prefix.io_file, env) == -1)
				return (-1);
		it = it->u_ast.s_cmd_prefix.cmd_prefix;
	}
	it = node->u_ast.s_simple_cmd.cmd_suffix;
	while (it)
	{
		if (it->type == AST_CMD_SUFFIX && it->u_ast.s_cmd_suffix.io_file
			&& it->u_ast.s_cmd_suffix.io_file->type == AST_IO_FILE)
			if (handle_heredoc_io(it->u_ast.s_cmd_suffix.io_file, env) == -1)
				return (-1);
		it = it->u_ast.s_cmd_suffix.cmd_suffix;
	}
	return (0);
}

static int	process_binary_heredoc(t_ast *left, t_ast *right, t_shell_env *env)
{
	if (process_heredocs(left, env) == -1)
		return (-1);
	return (process_heredocs(right, env));
}

int	process_heredocs(t_ast *node, t_shell_env *env)
{
	if (!node)
		return (0);
	if (node->type == AST_PIPE_SEQ)
		return (process_binary_heredoc(node->u_ast.s_pipe_seq.left,
				node->u_ast.s_pipe_seq.right, env));
	if (node->type == AST_AND_OR || node->type == AST_LIST)
		return (process_binary_heredoc(node->u_ast.s_and_or.left,
				node->u_ast.s_and_or.right, env));
	if (node->type == AST_SUBSHELL)
	{
		if (process_heredocs(node->u_ast.s_subshell.and_or, env) == -1)
			return (-1);
		return (handle_heredoc_io(node->u_ast.s_subshell.io_file, env));
	}
	if (node->type == AST_CMD_PREFIX)
	{
		if (handle_heredoc_io(node->u_ast.s_cmd_prefix.io_file, env) == -1)
			return (-1);
		return (process_heredocs(node->u_ast.s_cmd_prefix.cmd_prefix, env));
	}
	if (node->type == AST_SIMPLE_CMD)
		return (process_simple_cmd_heredocs(node, env));
	return (0);
}
