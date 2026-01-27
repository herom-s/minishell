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
#include <readline/readline.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static int	handle_io(t_ast *io)
{
	char	*temp_file;

	if (io && io->u_ast.s_io_file.op->type == DLESS)
	{
		temp_file = generate_heredoc_filename();
		if (write_heredoc_to_file((char *)io->u_ast.s_io_file.filename,
				temp_file) == -1)
		{
			free(temp_file);
			return (-1);
		}
		io->u_ast.s_io_file.op->type = LESS;
		io->u_ast.s_io_file.filename = temp_file;
	}
	return (0);
}

int	process_heredocs(t_ast *node, t_shell_env *env)
{
	t_ast	*tmp;

	if (!node)
		return (0);
	if (node->type == AST_PIPE_SEQ)
	{
		if (process_heredocs(node->u_ast.s_pipe_seq.left, env) == -1)
			return (-1);
		return (process_heredocs(node->u_ast.s_pipe_seq.right, env));
	}
	tmp = node->u_ast.s_simple_cmd.cmd_prefix;
	while (node->type == AST_SIMPLE_CMD && tmp)
	{
		if (handle_io(tmp->u_ast.s_cmd_prefix.io_file) == -1)
			return (-1);
		tmp = tmp->u_ast.s_cmd_prefix.cmd_prefix;
	}
	tmp = node->u_ast.s_simple_cmd.cmd_suffix;
	while (node->type == AST_SIMPLE_CMD && tmp)
	{
		if (handle_io(tmp->u_ast.s_cmd_suffix.io_file) == -1)
			return (-1);
		tmp = tmp->u_ast.s_cmd_suffix.cmd_suffix;
	}
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

void	cleanup_heredoc_files(t_ast *ast)
{
	t_ast	*curr;

	if (!ast)
		return ;
	if (ast->type == AST_PIPE_SEQ)
	{
		cleanup_heredoc_files(ast->u_ast.s_pipe_seq.left);
		cleanup_heredoc_files(ast->u_ast.s_pipe_seq.right);
	}
	else if (ast->type == AST_SIMPLE_CMD)
	{
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
}
