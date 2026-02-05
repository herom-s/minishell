/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:00:00 by hermarti          #+#    #+#             */
/*   Updated: 2026/02/05 14:00:00 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>

void	unlink_heredoc_node(t_ast *node)
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

void	free_heredoc_mem(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == AST_IO_FILE)
	{
		if (node->u_ast.s_io_file.filename
			&& ft_strncmp(node->u_ast.s_io_file.filename, "/tmp/.heredoc_",
				14) == 0)
		{
			free((char *)node->u_ast.s_io_file.filename);
			node->u_ast.s_io_file.filename = NULL;
		}
	}
}

void	cleanup_simple_cmd_heredocs(t_ast *ast)
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

void	free_simple_cmd_heredocs(t_ast *ast)
{
	t_ast	*curr;

	curr = ast->u_ast.s_simple_cmd.cmd_prefix;
	while (curr)
	{
		free_heredoc_mem(curr->u_ast.s_cmd_prefix.io_file);
		curr = curr->u_ast.s_cmd_prefix.cmd_prefix;
	}
	curr = ast->u_ast.s_simple_cmd.cmd_suffix;
	while (curr)
	{
		free_heredoc_mem(curr->u_ast.s_cmd_suffix.io_file);
		curr = curr->u_ast.s_cmd_suffix.cmd_suffix;
	}
}

void	free_heredoc_prefix(t_ast *prefix)
{
	t_ast	*curr;

	curr = prefix;
	while (curr)
	{
		free_heredoc_mem(curr->u_ast.s_cmd_prefix.io_file);
		curr = curr->u_ast.s_cmd_prefix.cmd_prefix;
	}
}
