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

int	handle_heredoc_io(t_ast *io);

static int	process_simple_cmd_heredocs(t_ast *node)
{
	t_ast	*it;

	it = node->u_ast.s_simple_cmd.cmd_prefix;
	while (it)
	{
		if (it->type == AST_CMD_PREFIX && it->u_ast.s_cmd_prefix.io_file
			&& it->u_ast.s_cmd_prefix.io_file->type == AST_IO_FILE)
			if (handle_heredoc_io(it->u_ast.s_cmd_prefix.io_file) == -1)
				return (-1);
		it = it->u_ast.s_cmd_prefix.cmd_prefix;
	}
	it = node->u_ast.s_simple_cmd.cmd_suffix;
	while (it)
	{
		if (it->type == AST_CMD_SUFFIX && it->u_ast.s_cmd_suffix.io_file
			&& it->u_ast.s_cmd_suffix.io_file->type == AST_IO_FILE)
			if (handle_heredoc_io(it->u_ast.s_cmd_suffix.io_file) == -1)
				return (-1);
		it = it->u_ast.s_cmd_suffix.cmd_suffix;
	}
	return (0);
}

int	process_heredocs(t_ast *node, t_shell_env *env)
{
	if (!node)
		return (0);
	if (node->type == AST_PIPE_SEQ)
	{
		if (process_heredocs(node->u_ast.s_pipe_seq.left, env) == -1)
			return (-1);
		return (process_heredocs(node->u_ast.s_pipe_seq.right, env));
	}
	if (node->type == AST_AND_OR || node->type == AST_LIST)
	{
		if (process_heredocs(node->u_ast.s_and_or.left, env) == -1)
			return (-1);
		return (process_heredocs(node->u_ast.s_and_or.right, env));
	}
	if (node->type == AST_SUBSHELL)
		return (process_heredocs(node->u_ast.s_subshell.and_or, env));
	if (node->type == AST_SIMPLE_CMD)
		return (process_simple_cmd_heredocs(node));
	return (0);
}

static void	unlink_heredoc_node(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == AST_IO_FILE)
	{
		if (node->u_ast.s_io_file.op->type == DLESS
			|| node->u_ast.s_io_file.op->type == LESS)
		{
			if (node->u_ast.s_io_file.filename
				&& ft_strncmp(node->u_ast.s_io_file.filename, "/tmp/.heredoc_",
					14) == 0)
			{
				unlink(node->u_ast.s_io_file.filename);
				free((char *)node->u_ast.s_io_file.filename);
				node->u_ast.s_io_file.filename = NULL;
			}
		}
	}
}

static void	cleanup_simple_cmd_heredocs(t_ast *ast)
{
	t_ast	*curr;

	curr = ast->u_ast.s_simple_cmd.cmd_prefix;
	while (curr)
	{
		unlink_heredoc_node(curr->u_ast.s_cmd_prefix.io_file);
		curr = curr->u_ast.s_cmd_prefix.cmd_prefix;
	}
	curr = ast->u_ast.s_simple_cmd.cmd_suffix;
	while (curr)
	{
		unlink_heredoc_node(curr->u_ast.s_cmd_suffix.io_file);
		curr = curr->u_ast.s_cmd_suffix.cmd_suffix;
	}
}

void	cleanup_heredoc_files(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->type == AST_PIPE_SEQ)
	{
		cleanup_heredoc_files(ast->u_ast.s_pipe_seq.left);
		cleanup_heredoc_files(ast->u_ast.s_pipe_seq.right);
	}
	else if (ast->type == AST_AND_OR || ast->type == AST_LIST)
	{
		cleanup_heredoc_files(ast->u_ast.s_and_or.left);
		cleanup_heredoc_files(ast->u_ast.s_and_or.right);
	}
	else if (ast->type == AST_SUBSHELL)
		cleanup_heredoc_files(ast->u_ast.s_subshell.and_or);
	else if (ast->type == AST_SIMPLE_CMD)
		cleanup_simple_cmd_heredocs(ast);
}
