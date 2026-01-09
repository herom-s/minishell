/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:37:26 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/09 14:13:05 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "libft.h"
#include "eval.h"
#include "token.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int	has_redirections(t_ast *shell_ast)
{
	t_ast	*suffix;
	t_ast	*prefix;

	prefix = shell_ast->u_ast.s_simple_cmd.cmd_prefix;
	while (prefix)
	{
		if (prefix->u_ast.s_cmd_prefix.io_file)
			return (1);
		prefix = prefix->u_ast.s_cmd_prefix.cmd_prefix;
	}
	suffix = shell_ast->u_ast.s_simple_cmd.cmd_suffix;
	while (suffix)
	{
		if (suffix->u_ast.s_cmd_suffix.io_file)
			return (1);
		suffix = suffix->u_ast.s_cmd_suffix.cmd_suffix;
	}
	return (0);
}

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

static int	process_io_file(t_ast *io_file, int *saved_stdin_fd)
{
	int	fd;
	int	target;

	if (!io_file)
		return (-1);
	if (io_file->u_ast.s_io_file.op->type == DLESS)
	{
		if (*saved_stdin_fd != -1)
			close(*saved_stdin_fd);
		return (handle_here_doc((char *)io_file->u_ast.s_io_file.filename, saved_stdin_fd));
	}
	
	fd = get_fd_for_op(io_file->u_ast.s_io_file.filename, io_file->u_ast.s_io_file.op->type);
	if (fd < 0)
	{
		ft_dprintf(STDERR_FILENO, "%s: %s\n", io_file->u_ast.s_io_file.filename, strerror(errno));
		return (-1);
	}
	if (io_file->u_ast.s_io_file.op->type == LESS)
	{
		target = STDIN_FILENO;
		if (*saved_stdin_fd != -1)
		{
			close(*saved_stdin_fd);
			*saved_stdin_fd = -1;
		}
	}
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

int	eval_redir(t_ast *shell_ast)
{
	t_ast	*suffix;
	t_ast	*prefix;
	int		saved_stdin_fd;

	saved_stdin_fd = -1;
	prefix = shell_ast->u_ast.s_simple_cmd.cmd_prefix;
	while (prefix)
	{
		if (process_io_file(prefix->u_ast.s_cmd_prefix.io_file, &saved_stdin_fd) < 0)
		{
			if (saved_stdin_fd != -1)
				close(saved_stdin_fd);
			return (-1);
		}
		prefix = prefix->u_ast.s_cmd_prefix.cmd_prefix;
	}
	suffix = shell_ast->u_ast.s_simple_cmd.cmd_suffix;
	while (suffix)
	{
		if (process_io_file(suffix->u_ast.s_cmd_suffix.io_file, &saved_stdin_fd) < 0)
		{
			if (saved_stdin_fd != -1)
				close(saved_stdin_fd);
			return (-1);
		}
		suffix = suffix->u_ast.s_cmd_suffix.cmd_suffix;
	}
	if (saved_stdin_fd != -1)
	{
		if (dup2(saved_stdin_fd, STDIN_FILENO) < 0)
		{
			close(saved_stdin_fd);
			return (-1);
		}
		close(saved_stdin_fd);
	}
	return (1);
}
