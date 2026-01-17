/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 13:25:56 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/15 13:26:21 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

static int	check_heredoc_in_simple_cmd(t_ast *shell_ast)
{
	t_ast	*suffix;
	t_ast	*prefix;
	t_ast	*io_file;

	prefix = shell_ast->u_ast.s_simple_cmd.cmd_prefix;
	while (prefix)
	{
		io_file = prefix->u_ast.s_cmd_prefix.io_file;
		if (io_file && io_file->u_ast.s_io_file.op
			&& io_file->u_ast.s_io_file.op->type == DLESS)
			return (1);
		prefix = prefix->u_ast.s_cmd_prefix.cmd_prefix;
	}
	suffix = shell_ast->u_ast.s_simple_cmd.cmd_suffix;
	while (suffix)
	{
		io_file = suffix->u_ast.s_cmd_suffix.io_file;
		if (io_file && io_file->u_ast.s_io_file.op
			&& io_file->u_ast.s_io_file.op->type == DLESS)
			return (1);
		suffix = suffix->u_ast.s_cmd_suffix.cmd_suffix;
	}
	return (0);
}

int	has_heredoc(t_ast *shell_ast)
{
	if (!shell_ast)
		return (0);
	if (shell_ast->type == AST_SIMPLE_CMD)
		return (check_heredoc_in_simple_cmd(shell_ast));
	if (shell_ast->type == AST_PIPE_SEQ)
	{
		if (has_heredoc(shell_ast->u_ast.s_pipe_seq.left))
			return (1);
		if (has_heredoc(shell_ast->u_ast.s_pipe_seq.right))
			return (1);
	}
	return (0);
}
