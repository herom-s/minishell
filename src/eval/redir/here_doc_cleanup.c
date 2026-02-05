/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_cleanup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:05:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/05 14:05:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include <stddef.h>

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
	{
		cleanup_heredoc_files(ast->u_ast.s_subshell.and_or);
		unlink_heredoc_node(ast->u_ast.s_subshell.io_file);
	}
	else if (ast->type == AST_CMD_PREFIX)
	{
		unlink_heredoc_node(ast->u_ast.s_cmd_prefix.io_file);
		cleanup_heredoc_files(ast->u_ast.s_cmd_prefix.cmd_prefix);
	}
	else if (ast->type == AST_SIMPLE_CMD)
		cleanup_simple_cmd_heredocs(ast);
}

void	free_heredoc_filenames(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->type == AST_PIPE_SEQ)
	{
		free_heredoc_filenames(ast->u_ast.s_pipe_seq.left);
		free_heredoc_filenames(ast->u_ast.s_pipe_seq.right);
	}
	else if (ast->type == AST_AND_OR || ast->type == AST_LIST)
	{
		free_heredoc_filenames(ast->u_ast.s_and_or.left);
		free_heredoc_filenames(ast->u_ast.s_and_or.right);
	}
	else if (ast->type == AST_SUBSHELL)
	{
		free_heredoc_filenames(ast->u_ast.s_subshell.and_or);
		free_heredoc_mem(ast->u_ast.s_subshell.io_file);
	}
	else if (ast->type == AST_CMD_PREFIX)
		free_heredoc_prefix(ast);
	else if (ast->type == AST_SIMPLE_CMD)
		free_simple_cmd_heredocs(ast);
}
