/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hermarti <hermarti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:37:26 by hermarti          #+#    #+#             */
/*   Updated: 2026/01/05 17:29:23 by hermarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "libft.h"
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

static int	open_dup2_io_file(const char *filename, t_token_type op)
{
	int	fd;
	int	target;

	fd = get_fd_for_op(filename, op);
	if (fd < 0)
	{
		ft_dprintf(STDERR_FILENO, "%s: %s\n", filename, strerror(errno));
		return (-1);
	}
	if (op == LESS || op == DLESS)
		target = STDIN_FILENO;
	else
		target = STDOUT_FILENO;
	if (dup2(fd, target) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (1);
}

static int	process_io_file(t_ast *io_file)
{
	if (!io_file)
		return (1);
	return (open_dup2_io_file(io_file->u_ast.s_io_file.filename,
			io_file->u_ast.s_io_file.op->type));
}

int	eval_redir(t_ast *shell_ast)
{
	t_ast	*suffix;
	t_ast	*prefix;

	prefix = shell_ast->u_ast.s_simple_cmd.cmd_prefix;
	while (prefix)
	{
		if (process_io_file(prefix->u_ast.s_cmd_prefix.io_file) < 0)
			return (-1);
		prefix = prefix->u_ast.s_cmd_prefix.cmd_prefix;
	}
	suffix = shell_ast->u_ast.s_simple_cmd.cmd_suffix;
	while (suffix)
	{
		if (process_io_file(suffix->u_ast.s_cmd_suffix.io_file) < 0)
			return (-1);
		suffix = suffix->u_ast.s_cmd_suffix.cmd_suffix;
	}
	return (1);
}
