/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:37:26 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/13 19:25:36 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "eval.h"
#include "libft.h"
#include "token.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int	get_fd_for_op(const char *filename, t_token_type op)
{
	if (op == GREAT)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (op == LESS)
		return (open(filename, O_RDONLY));
	if (op == DGREAT)
		return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (-1);
}

static int	handle_file_redir(t_ast *io)
{
	int		fd;
	int		target;
	char	*file;

	file = (char *)io->u_ast.s_io_file.filename;
	fd = get_fd_for_op(file, io->u_ast.s_io_file.op->type);
	if (fd < 0)
	{
		ft_dprintf(STDERR_FILENO, "%s: %s\n", file, strerror(errno));
		return (-1);
	}
	if (io->u_ast.s_io_file.op->type == LESS)
		target = STDIN_FILENO;
	else
		target = STDOUT_FILENO;
	if (dup2(fd, target) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	process_io_file(t_ast *io_file)
{
	if (!io_file)
		return (-1);
	return (handle_file_redir(io_file));
}

static int	process_list(t_ast *node, int is_suffix)
{
	t_ast	*io;

	while (node)
	{
		if (is_suffix)
			io = node->u_ast.s_cmd_suffix.io_file;
		else
			io = node->u_ast.s_cmd_prefix.io_file;
		if (io)
		{
			if (process_io_file(io) < 0)
				return (-1);
		}
		if (is_suffix)
			node = node->u_ast.s_cmd_suffix.cmd_suffix;
		else
			node = node->u_ast.s_cmd_prefix.cmd_prefix;
	}
	return (0);
}

int	eval_redir(t_ast *shell_ast)
{
	int	saved_stdin;
	int	ret;

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin < 0)
		return (-1);
	ret = process_list(shell_ast->u_ast.s_simple_cmd.cmd_prefix, 0);
	if (ret != -1)
		ret = process_list(shell_ast->u_ast.s_simple_cmd.cmd_suffix, 1);
	if (ret < 0)
	{
		close(saved_stdin);
		return (-1);
	}
	close(saved_stdin);
	return (1);
}
